#ifndef SGE_VIDEO_COLOR_HPP
#define SGE_VIDEO_COLOR_HPP

#include "sge/debug/assert.hpp"

#include <SDL_pixels.h>

#include <utility>
#include <cstdint>
#include <iterator>
#include <ostream>

namespace sge::video {

namespace detail {
template <typename Derived>
struct color_api {
    inline std::uint8_t& red() noexcept { return to_underlying_type(*this)->r; }
    inline std::uint8_t red() const noexcept { return to_underlying_type(*this)->r; }

    inline std::uint8_t& green() noexcept { return to_underlying_type(*this)->g; }
    inline std::uint8_t green() const noexcept { return to_underlying_type(*this)->g; }

    inline std::uint8_t& blue() noexcept { return to_underlying_type(*this)->b; }
    inline std::uint8_t blue() const noexcept { return to_underlying_type(*this)->b; }

    inline std::uint8_t& alpha() noexcept { return to_underlying_type(*this)->a; }
    inline std::uint8_t alpha() const noexcept { return to_underlying_type(*this)->a; }

    inline friend auto to_underlying_type(color_api& c) noexcept {
        return to_underlying_type(static_cast<Derived&>(c));
    }
    inline friend auto to_underlying_type(const color_api& c) noexcept {
        return to_underlying_type(static_cast<const Derived&>(c));
    }

protected:
    ~color_api() noexcept = default;
};

template <typename Derived1, typename Derived2>
bool operator==(const color_api<Derived1>& lhs, const color_api<Derived2>& rhs) noexcept {
    return (lhs.red() == rhs.red()) && (lhs.green() == rhs.green()) && (lhs.blue() == rhs.blue()) &&
           (lhs.alpha() == rhs.alpha());
}
template <typename Derived>
std::ostream& operator<<(std::ostream& stream, const color_api<Derived>& color) {
    stream << static_cast<int>(color.red()) << " : " << static_cast<int>(color.green()) << " : "
           << static_cast<int>(color.blue()) << " : " << static_cast<int>(color.alpha());
    return stream;
}
}  // namespace detail

struct color final : public detail::color_api<color> {
    explicit color(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0, uint8_t a = 0xFF) : m_color{r, g, b, a} {}

    template <typename Derived>
    color(const detail::color_api<Derived>& c) noexcept : m_color{c.red(), c.green(), c.blue(), c.alpha()} {}

    template <typename Derived>
    inline color& operator=(const detail::color_api<Derived>& c) noexcept {
        color(c).swap(*this);
        return *this;
    }

    color(color&& c) noexcept : m_color(std::move(c.m_color)) {}
    inline color& operator=(color&& c) noexcept {
        color(std::move(c)).swap(*this);
        return *this;
    }

    inline void swap(color& c) noexcept {
        using std::swap;
        swap(m_color, c.m_color);
    }

private:
    explicit color(const SDL_Color& c) : m_color{c.r, c.g, c.b, c.a} {}

    SDL_Color m_color;

    friend SDL_Color* to_underlying_type(color&) noexcept;
    friend const SDL_Color* to_underlying_type(const color&) noexcept;
};

namespace constant_color {
static inline const color transparent{0xFF, 0xFF, 0xFF, 0x00};
static inline const color white{0xFF, 0xFF, 0xFF, 0xFF};
}  // namespace constant_color

inline void swap(color& lhs, color& rhs) noexcept { lhs.swap(rhs); }

inline SDL_Color* to_underlying_type(color& c) noexcept { return (&c.m_color); }
inline const SDL_Color* to_underlying_type(const color& c) noexcept { return (&c.m_color); }

struct color_ref final : public detail::color_api<color_ref> {
    explicit color_ref(SDL_Color& c) noexcept : m_color(c) {}
    explicit color_ref(color& c) noexcept : m_color(*to_underlying_type(c)) {}

    template <typename Derived>
    inline color_ref& operator=(const detail::color_api<Derived>& c) noexcept {
        this->red()   = c.red();
        this->blue()  = c.blue();
        this->green() = c.green();
        this->alpha() = c.alpha();

        return *this;
    }

private:
    SDL_Color& m_color;

    friend SDL_Color* to_underlying_type(color_ref&) noexcept;
    friend const SDL_Color* to_underlying_type(const color_ref&) noexcept;
};

inline auto to_underlying_type(color_ref& c) noexcept -> SDL_Color* { return (&c.m_color); }
inline auto to_underlying_type(const color_ref& c) noexcept -> const SDL_Color* { return (&c.m_color); }

struct const_color_ref final : public detail::color_api<color_ref> {
    explicit const_color_ref(const SDL_Color& c) noexcept : m_color(c) {}
    explicit const_color_ref(const color& c) noexcept : m_color(*to_underlying_type(c)) {}

private:
    const SDL_Color& m_color;

    friend const SDL_Color* to_underlying_type(const const_color_ref&) noexcept;
};

inline auto to_underlying_type(const const_color_ref& c) noexcept -> const SDL_Color* { return (&c.m_color); }

template <std::size_t SIZE>
class color_array final {
public:
    using size_type = std::size_t;

    using reference       = color_ref;
    using const_reference = const_color_ref;

    template <typename Derived>
    constexpr void fill(const detail::color_api<Derived>& value) noexcept {
        std::fill(std::begin(m_colors), std::end(m_colors),
                  SDL_Color{value.red(), value.green(), value.blue(), value.alpha()});
    }

    constexpr size_type size() const noexcept { return SIZE; }
    constexpr bool empty() const noexcept { return size() == 0; }

    auto operator[](size_type pos) noexcept -> reference {
        sge_expects(pos < SIZE);
        return reference(m_colors[pos]);
    }
    auto operator[](size_type pos) const noexcept -> const_reference {
        sge_expects(pos < SIZE);
        return const_reference(m_colors[pos]);
    }

private:
    SDL_Color m_colors[SIZE];
};

}  // namespace sge::video

#endif  // SGE_VIDEO_COLOR_HPP
