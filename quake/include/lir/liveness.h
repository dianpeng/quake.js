#ifndef _QUAKE_LIR_LIVENESS_H_
#define _QUAKE_LIR_LIVENESS_H_
#include "base.h"
#include "zone.h"

namespace quake {
namespace lir {

// Indicating the liveness is not used anymore
constexpr std::uint32_t kInvalidLivenessId = 0;

// Each instruction will oocupies 2 consequtive liveness id,
// Its input will use x, and its output will use x+1
constexpr std::uint32_t kSingleInstructionLivenessStep = 2;

constexpr std::uint32_t kMinLivenessId = 2;

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

  LivenessPoint Prev() const { return LivenessPoint{point_ - 1}; }
  LivenessPoint Next() const { return LivenessPoint{point_ + 1}; }

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
  static LivenessRange StartUntilLast(const LivenessPoint& x) {
    return LivenessRange{x.point(), kMaxLivenessId};
  }
  static LivenessRange OneInstruction(const LivenessPoint& start) {
    return LivenessRange{start.point(), start.point() + 1};
  }
  static LivenessRange MaxRange() {
    return LivenessRange{kMinLivenessId, kMaxLivenessId};
  }

 public:
  LivenessRange() = default;
  LivenessRange(std::uint32_t start, std::uint32_t end)
      : start_(start), end_(end) {}

  LivenessRange(const LivenessPoint& start, const LivenessPoint& end)
      : start_(start), end_(end) {}
  const LivenessPoint& start() const { return start_; }
  const LivenessPoint& end() const { return end_; }

  LivenessPoint& start() { return start_; }
  LivenessPoint& end() { return end_; }

  void EndBefore(const LivenessPoint& when) { end_ = when.Prev(); }

  bool IsValid() const { return start().IsValid() && end().IsValid(); }
  bool Contains(const LivenessPoint& that) const {
    return Contains(that.point());
  }
  bool Contains(std::uint32_t point) const {
    return start_.point() <= point && point <= end_.point();
  }

  std::string Print() const {
    return base::Fmt("<%d, %d>", start_.point(), end_.point());
  }

 public:
  // Special liveness point

  // Use point, ie instruction's operand's reference liveness point (ts)
  LivenessPoint use_point() const { return start(); }

  // Outpoint point, ie instruction's output reference liveness point (ts)
  LivenessPoint output_point() const { return use_point().Next(); }

 private:
  LivenessPoint start_;
  LivenessPoint end_;
};

class LivenessInfo {
 public:
  using UseList = zone::Vector<LivenessPoint>;

  explicit LivenessInfo(zone::Zone* zone)
      : liveness_id_(kInvalidLivenessId), range_(), use_list_(zone) {}

  LivenessPoint& def() { return range_.start(); }
  const LivenessPoint& def() const { return range_.start(); }

  LivenessPoint& end() { return range_.end(); }
  const LivenessPoint& end() const { return range_.end(); }

  LivenessRange& range() { return range_; }
  const LivenessRange& range() const { return range_; }

  inline const LivenessPoint& last_use() const;

 public:
  // Used by RA
  inline void SetLivenessStart(std::uint32_t);
  UseList& use_list() { return use_list_; }
  const UseList& use_list() const { return use_list_; }
  inline void AddUseBack(LivenessPoint&&);
  inline void SetNoUse();

  // return the next use *equal or after* the specified point if applicable
  std::optional<LivenessPoint> NextUse(const LivenessPoint&) const;

 public:
  void set_liveness_id(std::uint32_t id) { liveness_id_ = id; }
  std::uint32_t liveness_id() const { return liveness_id_; }
  bool HasLivenessId() const { return liveness_id_ != kInvalidLivenessId; }

 public:
  bool IsValid() const { return HasLivenessId() && range().IsValid(); }

  std::string Print() const;

 private:
  std::uint32_t liveness_id_;
  LivenessRange range_;
  UseList use_list_;
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
  if (!end().IsValid()) {
    end() = p;
  }
  auto itr = std::upper_bound(use_list_.begin(), use_list_.end(), p);
  use_list_.insert(itr, std::move(p));
}

inline void LivenessInfo::SetNoUse() {
  if (!end().IsValid()) {
    end().set_point(def().point() + 1);
  }
}

inline const LivenessPoint& LivenessInfo::last_use() const {
  if (use_list_.empty()) {
    return end();
  } else {
    return use_list_.back();
  }
}

}  // namespace lir
}  // namespace quake

#endif  // _QUAKE_LIR_LIVENESS_H_
