#ifndef _QUAKE_LIR_LIVENESS_H_
#define _QUAKE_LIR_LIVENESS_H_
#include "base.h"
#include "zone.h"

namespace quake {
namespace lir {

// Indicating the liveness is not used anymore
constexpr std::uint32_t kInvalidLivenessId = 0;

// The largest liveness id
constexpr std::uint32_t kMaxLivenessId =
    std::numeric_limits<std::uint32_t>::max();

class LivenessPoint {
 public:
  LivenessPoint() = default;

  // We purposely allow implicit conversion
  /* explicit */ LivenessPoint(std::uint32_t p) : point_(p) {}

  bool IsValid() const { return point_ != kInvalidLivenessId; }

  std::uint32_t point() const { return point_; }
  void set_point(std::uint32_t l) { point_ = l; }
  operator std::uint32_t() const { return point_; }

  inline bool operator==(const LivenessPoint&) const;
  inline bool operator!=(const LivenessPoint&) const;
  inline bool operator<=(const LivenessPoint&) const;
  inline bool operator<(const LivenessPoint&) const;
  inline bool operator>=(const LivenessPoint&) const;
  inline bool operator>(const LivenessPoint&) const;

 private:
  std::uint32_t point_ = kInvalidLivenessId;
};

class LivenessRange {
 public:
  LivenessRange() = default;
  LivenessRange(std::uint32_t start, std::uint32_t end)
      : start_(start), end_(end) {}

  LivenessRange(const LivenessPoint& start, const LivenessPoint& end)
      : start_(start), end_(end) {}

  static StartUntilLast(const LivenessPoint& x) {
    return LivenessRange{x.point(), kMaxLivenessId};
  }

  const LivenessPoint& start() const { return start_; }
  const LivenessPoint& end() const { return end_; }

  LivenessPoint& start() { return start_; }
  LivenessPoint& end() { return end_; }

  bool IsValid() const { return start().IsValid() && end().IsValid(); }

 private:
  LivenessPoint start_;
  LivenessPoint end_;
};

class LivenessInfo {
 public:
  using UseList = zone::Vector<LivenessPoint>;

  explicit LivenessInfo(zone::Zone* zone) : range_(), use_list_(zone) {}

  LivenessPoint& def() { return range_.start(); }
  const LivenessPoint& def() const { return range_.start(); }

  LivenessPoint& last_use() { return range_.end(); }
  const LivenessPoint& last_use() const { return range_.end(); }

  LivenessRange& range() { return range_; }
  const LivenessRange& range() const { return range_; }

 public:
  // Used by RA
  inline void SetLivenessStart(std::uint32_t);
  UseList& use_list() { return use_list_; }
  const UseList& use_list() const { return use_list_; }
  inline void AddUseBack(LivenessPoint&&);

 public:
  void set_liveness_id(std::uint32_t id) const { liveness_id_ = id; }
  std::uint32_t liveness_id() const { return liveness_id_; }
  bool HasLivenessId() const { return liveness_id_ != kInvalidLivenessId; }

 public:
  bool IsValid() const { return HasLivenessId() && range().IsValid(); }

 private:
  std::uint32_t liveness_id_ = kInvalidLivenessId;
  LivenessRange range_{};
  UseList use_list_{};
};

inline bool LivenessPoint::operator==(const LivenessPoint& t) const {
  return point_ == t.point_;
}

inline bool LivenessPoint::operator!=(const LivenessPoint& t) const {
  return point_ != t.point_;
}

inline bool LivenessPoint::operator<=(const LivenessPoint& t) const {
  return point_ <= t.point_;
}

inline bool LivenessPoint::operator<(const LivenessPoint& t) const {
  return point_ < t.point_;
}

inline bool LivenessPoint::operator>=(const LivenessPoint& t) const {
  return point_ >= t.point_;
}

inline bool LivenessPoint::operator>(const LivenessPoint& t) const {
  return point_ > t.point_;
}

inline void LivenessInfo::SetLivenessStart(std::uint32_t x) {
  CHECK(liveness_id_ == kInvalidLivenessId);
  CHECK(!def().IsValid());

  liveness_id_ = x;
  def().set_point(x);
}

inline void LivenessInfo::AddUseBack(LivenessPoint&& p) {
  BASE_DBG({
    if (def().IsValid()) CHECK(def() < p);
  });

  if (!last_use().HasLivenessId()) {
    last_use() = p;
  }
  auto itr = std::upper_bound(use_list_.begin(), use_lsit_.end(), p);
  use_list_.insert(itr, std::move(p));
}

}  // namespace lir
}  // namespace quake

#endif  // _QUAKE_LIR_LIVENESS_H_
