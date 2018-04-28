#include "potential.hpp"

namespace potential {
using pammap::All;
using pammap::ArrayView;

PotentialState::PotentialState(pammap::PamMap p)
      : charges{p.at<ArrayView<Float>>("charges")},
        positions{p.at<ArrayView<Float>>("positions")} {
  // TODO Bounds checks and exceptions
}

Float diff_squared(const ArrayView<Float>& p, const ArrayView<Float>& q) {
  // TODO Bounds checks and exceptions
  Float acc = 0;
  for (size_t i = 0; i < p.size(); ++i) {
    acc += (p[i] - q[i]) * (p[i] - q[i]);
  }
  return acc;
}

void compute(const PotentialState& state, ArrayView<Float> gridpoints,
             ArrayView<Float> out_values) {
  // TODO Bounds checks and exceptions

  const size_t n_charges = state.charges.shape()[0];
  const size_t n_points  = gridpoints.shape()[0];

  for (size_t p = 0; p < n_points; ++p) {
    out_values[p]                = 0;
    const ArrayView<Float> point = gridpoints.slice({{p}, All});
    for (size_t c = 0; c < n_charges; ++c) {
      const ArrayView<Float> pos = state.positions.slice({c, All});
      out_values[p] += state.charges[c] / std::sqrt(diff_squared(pos, point));
    }
  }
}
}  // namespace potential
