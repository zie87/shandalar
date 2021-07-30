#ifndef SGE_VIDEO_IMAGEPIC_HPP
#define SGE_VIDEO_IMAGEPIC_HPP

#include "color.hpp"

#include <istream>
#include <optional>

namespace sge::video {
    using color_array_result = std::optional<sge::video::color_array<256>>;

    color_array_result parse_palette_tr(std::istream& input) noexcept;
}

#endif // SGE_VIDEO_IMAGEPIC_HPP
