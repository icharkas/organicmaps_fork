#pragma once

#include "partners_api/booking_availability_params.hpp"
#include "partners_api/booking_params_base.hpp"

#include "platform/safe_callback.hpp"

#include "indexer/feature_decl.hpp"

#include <functional>
#include <memory>
#include <utility>
#include <vector>

namespace search
{
class Results;
}

namespace booking
{
namespace filter
{
using Results = platform::SafeCallback<void(std::shared_ptr<ParamsBase> const & params,
                                            std::vector<FeatureID> const & sortedFeatures)>;
using ResultsUnsafe = std::function<void(search::Results const & results)>;

template <typename R>
struct ParamsImpl
{
  ParamsImpl() = default;
  ParamsImpl(std::shared_ptr<ParamsBase> apiParams, R const & cb)
    : m_apiParams(apiParams)
    , m_callback(cb)
  {
  }

  bool IsEmpty() const { return !m_apiParams || m_apiParams->IsEmpty(); }

  std::shared_ptr<ParamsBase> m_apiParams;
  R m_callback;
};

using Params = ParamsImpl<Results>;
using ParamsInternal = ParamsImpl<ResultsUnsafe>;

enum class Type
{
  Deals,
  Availability
};

using Types = std::vector<Type>;

template <typename T>
struct TaskImpl
{
  TaskImpl(Type const type, T && filterParams)
    : m_type(type)
    , m_filterParams(std::move(filterParams))
  {
  }

  Type m_type;
  T m_filterParams;
};

using Task = TaskImpl<Params>;
using TaskInternal = TaskImpl<ParamsInternal>;

enum ApplyMode
{
  Independent,
  Consecutively
};

template <typename T>
class TasksImpl
{
public:
  using Iter = std::vector<Task>::iterator;
  using ConstIter = std::vector<Task>::const_iterator;

  TasksImpl() = default;
  explicit TasksImpl(ApplyMode const mode) : m_applyMode(mode) {}

  Iter begin() { return m_tasks.begin(); }
  Iter end() { return m_tasks.end(); }
  ConstIter begin() const { return m_tasks.cbegin(); }
  ConstIter end() const { return m_tasks.cend(); }

  Iter Find(Type const type)
  {
    return find_if(m_tasks.begin(), m_tasks.end(),[type](Task const & task)
    {
      return task.m_type == type;
    });
  }

  ConstIter Find(Type const type) const
  {
    return find_if(m_tasks.cbegin(), m_tasks.cend(),[type](Task const & task)
    {
      return task.m_type == type;
    });
  }

  void Clear()
  {
    m_tasks.clear();
  }

  template <typename ... Args>
  void EmplaceBack(Args && ... args)
  {
    m_tasks.emplace_back(std::forward<Args>(args)...);
  }

  ApplyMode GetMode() const
  {
    return m_applyMode;
  }

private:
  std::vector<T> m_tasks;
  ApplyMode m_applyMode = ApplyMode::Independent;
};

using Tasks = TasksImpl<Task>;
using TasksInternal = std::vector<TaskInternal>;
}  // namespace filter
}  // namespace booking
