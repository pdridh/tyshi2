#pragma once

#include <vector>
#include <unordered_map>
#include <limits>
#include <algorithm>
#include <bitset>
#include <memory>

// Max amount of components at once.
constexpr size_t MAX_COMPONENTS = 64;

using EntityID = uint64_t;
using ComponentMask = std::bitset<MAX_COMPONENTS>;
using Sparse = std::vector<size_t>;

static constexpr EntityID NULL_ENTITY = std::numeric_limits<EntityID>::max();

class ISparseSet
{
public:
  virtual ~ISparseSet() = default;
  virtual void remove(EntityID id) = 0;
  virtual void clear() = 0;
};

template <typename T>
class SparseSet : public ISparseSet
{
public:
  SparseSet()
  {
    m_packed.reserve(100);
  }

  T *set(EntityID id, T obj)
  {
    size_t index = getPackedIndex(id);
    if (index != NULL_INDEX)
    {
      m_packed[index] = obj;
      m_packedToEntity[index] = id;

      return &m_packed[index];
    }

    setPackedIndex(id, m_packed.size());

    m_packed.push_back(obj);
    m_packedToEntity.push_back(id);

    return &m_packed.back();
  }

  T *get(EntityID id)
  {
    size_t index = getPackedIndex(id);
    return (index != NULL_INDEX) ? &m_packed[index] : nullptr;
  }

  void remove(EntityID id) override
  {
    size_t indexToRemove = getPackedIndex(id);

    setPackedIndex(m_packedToEntity.back(), indexToRemove);
    setPackedIndex(id, NULL_INDEX);

    std::swap(m_packed.back(), m_packed[indexToRemove]);
    std::swap(m_packedToEntity.back(), m_packedToEntity[indexToRemove]);

    m_packed.pop_back();
    m_packedToEntity.pop_back();
  }

  void clear()
  {
    m_packed.clear();
    m_sparsePages.clear();
    m_packedToEntity.clear();
  }

  bool empty() const
  {
    return m_packed.empty();
  }

  const std::vector<T> &packed() const
  {
    return m_packed;
  }

private:
  void setPackedIndex(EntityID id, size_t index)
  {
    size_t page = id / SPARSE_MAX_SIZE;
    size_t sparseIndex = id % SPARSE_MAX_SIZE;

    if (page >= m_sparsePages.size())
      m_sparsePages.resize(page + 1);

    Sparse &sparse = m_sparsePages[page];
    if (sparseIndex >= sparse.size())
      sparse.resize(sparseIndex + 1, NULL_INDEX);

    sparse[sparseIndex] = index;
  }

  size_t getPackedIndex(EntityID id)
  {
    size_t page = id / SPARSE_MAX_SIZE;
    size_t sparseIndex = id % SPARSE_MAX_SIZE;

    if (page < m_sparsePages.size())
    {
      Sparse &sparse = m_sparsePages[page];
      if (sparseIndex < sparse.size())
        return sparse[sparseIndex];
    }

    return NULL_INDEX;
  }

private:
  const size_t SPARSE_MAX_SIZE = 1000;
  static constexpr size_t NULL_INDEX = std::numeric_limits<size_t>::max();

  std::vector<Sparse> m_sparsePages;
  std::vector<T> m_packed;
  std::vector<EntityID> m_packedToEntity;
};

class ECS
{
private:
  // List of IDs already created, but no longer in use
  std::vector<EntityID> m_availableEntities;

  // Holds the component mask for an entity
  SparseSet<ComponentMask> m_entityMasks;

  // Groups entities that share a component mask.
  std::unordered_map<ComponentMask, SparseSet<EntityID>> m_groupings;

  // Pools of components
  std::vector<std::unique_ptr<ISparseSet>> m_componentPools;

  // Key is component name, value is the bit position in ComponentMask
  std::unordered_map<const char *, size_t> m_componentBitPosition;

  // Highest recorded entity ID
  EntityID m_maxEntityID = 0;

  static constexpr size_t NULL_INDEX = std::numeric_limits<size_t>::max();

private:
  // Get position of the bit in ComponentMask for each component
  // based on the component template typeid
  template <typename Component>
  size_t getComponentBitPosition()
  {
    const char *name = typeid(Component).name();
    auto it = m_componentBitPosition.find(name);
    return it->second;
  }

  // Get reference for the specific component pool given a component name
  template <typename Component>
  SparseSet<Component> &getComponentPool()
  {
    size_t bitPos = getComponentBitPosition<Component>();

    // Downcast the generic pointer to the specific sparse set
    ISparseSet *genericPtr = m_componentPools[bitPos].get();
    SparseSet<Component> *pool = dynamic_cast<SparseSet<Component> *>(genericPtr);

    return *pool;
  }

  template <typename Component>
  void setComponentBit(ComponentMask &mask, bool val)
  {
    size_t bitPos = getComponentBitPosition<Component>();
    mask[bitPos] = val;
  }

  ComponentMask &getEntityMask(EntityID id)
  {
    ComponentMask *mask = m_entityMasks.get(id);
    return *mask;
  }

  /*
   *  Assembles a generic mask for the given components
   */
  template <typename... Components>
  ComponentMask getMask()
  {
    ComponentMask mask;
    (setComponentBit<Components>(mask, 1), ...); // fold expression
    return mask;
  }

  // Removes an entity from it's group with its current mask
  void removeFromGroup(ComponentMask &mask, EntityID id)
  {
    if (mask.none())
      return;

    SparseSet<EntityID> &group = getGroupedEntities(mask);
    group.remove(id);

    // Delete grouping if it's empty
    if (group.empty())
      m_groupings.erase(mask);
  }

  // Assign an entity to the component group
  void assignToGroup(ComponentMask &mask, EntityID id)
  {
    // If mask is empty, no group
    if (mask.none())
      return;

    // Create group if it doesn't exist
    m_groupings.emplace(std::piecewise_construct,
                        std::forward_as_tuple(mask),
                        std::forward_as_tuple()); // Empty sparse set
    m_groupings[mask].set(id, id);
  }

  // Get reference to the all entity ids are in this group
  SparseSet<EntityID> &getGroupedEntities(ComponentMask &mask)
  {
    auto found = m_groupings.find(mask);
    return found->second;
  }

public:
  ECS() = default;

  EntityID createEntity()
  {
    EntityID id = NULL_INDEX;

    if (m_availableEntities.size() == 0)
    {
      id = m_maxEntityID++;
    }
    else
    {
      id = m_availableEntities.back();
      m_availableEntities.pop_back();
    }

    m_entityMasks.set(id, {});

    return id;
  }

  void removeEntity(EntityID &id)
  {

    ComponentMask &mask = getEntityMask(id);

    // Remove from group
    removeFromGroup(mask, id);

    // Destroy component associations
    for (int i = 0; i < MAX_COMPONENTS; i++)
    {
      if (mask[i] == 1)
      {
        m_componentPools[i]->remove(id);
      }
    }

    m_entityMasks.remove(id);
    m_availableEntities.push_back(id);

    id = NULL_ENTITY;
  }

  // Register a component and create a pool for it
  template <typename Component>
  void registerComponent()
  {
    const char *name = typeid(Component).name();
    m_componentBitPosition.emplace(name, m_componentPools.size());
    m_componentPools.push_back(std::make_unique<SparseSet<Component>>());
  }

  // Add a component to an entity
  template <typename Component>
  Component &add(EntityID id, Component &&component = {})
  {
    SparseSet<Component> &pool = getComponentPool<Component>();

    ComponentMask &mask = getEntityMask(id);

    // Remove from old group
    removeFromGroup(mask, id);

    setComponentBit<Component>(mask, 1);

    // Add ID to new group
    assignToGroup(mask, id);

    return *pool.set(id, std::move(component));
  }

  // Get the specified component for the given entity
  template <typename Component>
  Component &get(EntityID id)
  {

    SparseSet<Component> &pool = getComponentPool<Component>();
    Component *component = pool.get(id);
    return *component;
  }

  //  Removes a component from an entity
  template <typename Component>
  void remove(EntityID id)
  {
    SparseSet<Component> &pool = getComponentPool<Component>();
    ComponentMask &mask = getEntityMask(id);

    // Remove from old group
    removeFromGroup(mask, id);

    setComponentBit<Component>(mask, 0);

    // Shift to new group with new mask
    assignToGroup(mask, id);
    pool.remove(id);
  }

  template <typename... Components>
  bool hasAll(EntityID id)
  {
    return (has<Components>(id) && ...);
  }

  template <typename Component>
  bool has(EntityID id)
  {
    SparseSet<Component> &pool = getComponentPool<Component>();
    return pool.get(id) ? true : false;
  }

  // Gets all the entity IDs matching the component parameter pack
  template <typename... Components>
  std::vector<EntityID> viewIDs()
  {
    std::vector<EntityID> result;
    ComponentMask targetMask = getMask<Components...>();

    for (auto &[mask, ids] : m_groupings)
    {
      // As long as a grouping contains all of the components of the target mask, return the IDs
      if ((mask & targetMask) == targetMask)
        result.insert(result.end(), ids.packed().begin(), ids.packed().end());
    }

    return result;
  }

  //  Returns tuple containing the id of the entity
  //  and all  valid components matching the passed parameter pack
  template <typename... Components>
  std::vector<std::tuple<EntityID, Components &...>> view()
  {
    std::vector<std::tuple<EntityID, Components &...>> result;
    ComponentMask targetMask = getMask<Components...>();

    for (auto &[mask, ids] : m_groupings)
    {
      if ((mask & targetMask) == targetMask)
      {
        for (EntityID id : ids.packed())
        {
          result.emplace_back(id, get<Components>(id)...);
        }
      }
    }

    return result;
  }

  /*
   *  Executes a passed lambda on all the entities that match the
   *  passed parameter pack.
   *
   *  Provided function should follow one of two forms:
   *  [](Component& c1, Component& c2);
   *  OR
   *  [](EntityID id, Component& c1, Component& c2);
   */
  template <typename... Components, typename Func>
  void forEach(Func &&func)
  {
    std::vector<std::tuple<EntityID, Components &...>> result;
    ComponentMask targetMask = getMask<Components...>();

    for (auto &[mask, ids] : m_groupings)
    {
      if ((mask & targetMask) == targetMask)
      {
        for (EntityID id : ids.packed())
        {

          // This branch is for [](EntityID id, Component& c1, Component& c2);
          if constexpr (std::is_invocable_v<Func, EntityID, Components &...>)
          {
            func(id, get<Components>(id)...);
          }

          // This branch is for [](Component& c1, Component& c2);
          else if constexpr (std::is_invocable_v<Func, Components &...>)
          {
            func(get<Components>(id)...);
          }
        }
      }
    }
  }
};
