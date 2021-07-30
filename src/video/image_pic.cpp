#include <sge/video/image_pic.hpp>
#include <sge/debug/assert.hpp>

#include <regex>
#include <string>
#include <charconv>
#include <optional>

namespace sge::video {

namespace {

template <typename T, typename U>
constexpr T narrow_cast(U u) noexcept {
    return static_cast<T>(u);
}

using color_result = std::optional<color>;

color_result parse_line_without_index(const std::string& line) noexcept {
    const std::regex expr("^\\s*(\\d+)\\s+(\\d+)\\s+(\\d+)\\s+-.*\\s*$");

    std::smatch sm;
    if (!std::regex_match(line, sm, expr) || (sm.size() != 4)) {
        return {};
    }

    auto get_value = [](const std::string& str, int& value) -> bool {
        const auto result = std::from_chars(str.data(), str.data() + str.size(), value);
        return (result.ec != std::errc::invalid_argument);
    };

    constexpr std::size_t num_values = 3;
    int values[num_values];
    for (std::size_t idx = 0; idx < num_values; ++idx) {
        if (!get_value(sm[(idx + 1)].str(), values[idx])) {
            return {};
        }
    }

    return color(narrow_cast<std::uint8_t>(values[0]), narrow_cast<std::uint8_t>(values[1]),
                 narrow_cast<std::uint8_t>(values[2]));
}

color_result parse_line_with_index(const std::string& line, std::size_t& index) noexcept {
    const std::regex expr("^\\s*(\\d+)\\s+-\\s+(\\d+)\\s+(\\d+)\\s+(\\d+)\\s+-.*\\s*$");

    std::smatch sm;
    if (!std::regex_match(line, sm, expr) || (sm.size() != 5)) {
        return {};
    }

    auto get_value = [](const std::string& str, int& value) -> bool {
        const auto result = std::from_chars(str.data(), str.data() + str.size(), value);
        return (result.ec != std::errc::invalid_argument);
    };

    constexpr std::size_t num_values = 4;
    int values[num_values];
    for (std::size_t idx = 0; idx < num_values; ++idx) {
        if (!get_value(sm[(idx + 1)].str(), values[idx])) {
            return {};
        }
    }

    index = narrow_cast<std::size_t>(values[0]);
    return color(narrow_cast<std::uint8_t>(values[1]), narrow_cast<std::uint8_t>(values[2]),
                 narrow_cast<std::uint8_t>(values[3]));
}

bool line_has_index(const std::string& line) {
    const auto format = std::count(line.begin(), line.end(), '-');
    sge_ensures((format == 1u) || (format == 2u));
    return format == 2u;
}

using color_array_result = std::optional<sge::video::color_array<256>>;

}  // namespace

color_array_result parse_palette_tr(std::istream& input) noexcept {
    sge::video::color_array<256> colors;
    colors.fill(sge::video::constant_color::transparent);

    std::string line;
    std::size_t index = 0;
    while (std::getline(input, line)) {
        if (line_has_index(line)) {
            const auto result = parse_line_with_index(line, index);
            if (!result.has_value()) {
                return {};
            }
            colors[index] = result.value();
        } else {
            const auto result = parse_line_without_index(line);
            if (!result.has_value()) {
                return {};
            }
            colors[index++] = result.value();
        }
    }

    return colors;
}

}  // namespace sge::video
