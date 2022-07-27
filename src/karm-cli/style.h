#pragma once

#include <karm-fmt/fmt.h>

namespace Karm::Cli {

enum Color {
    _COLOR_UNDEF = -1,

    BLACK = 0,
    RED = 1,
    GREEN = 2,
    YELLOW = 3,
    BLUE = 4,
    MAGENTA = 5,
    CYAN = 6,
    GRAY_LIGHT = 7,

    GRAY_DARK = 60,
    RED_LIGHT = 61,
    GREEN_LIGHT = 62,
    YELLOW_LIGHT = 63,
    BLUE_LIGHT = 64,
    MAGENTA_LIGHT = 65,
    CYAN_LIGHT = 66,
    WHITE = 67,
};

struct Style {
    bool _reset{};
    Color _fg{_COLOR_UNDEF};
    Color _bg{_COLOR_UNDEF};
    bool _bold{};
    bool _underline{};
    bool _blink{};
    bool _reverse{};
    bool _invisible{};

    constexpr Style() = default;

    constexpr Style(Color fg)
        : _fg(fg) {
    }

    constexpr Style(Color fg, Color bg)
        : _fg(fg), _bg(bg) {
    }

    constexpr Style fg(Color color) {
        _fg = color;
        return *this;
    }

    constexpr Style bg(Color color) {
        _bg = color;
        return *this;
    }

    constexpr Style bold() {
        _bold = true;
        return *this;
    }

    constexpr Style underline() {
        _underline = true;
        return *this;
    }

    constexpr Style blink() {
        _blink = true;
        return *this;
    }

    constexpr Style reverse() {
        _reverse = true;
        return *this;
    }

    constexpr Style invisible() {
        _invisible = true;
        return *this;
    }

    constexpr Style reset() {
        _reset = true;
        return *this;
    }
};

[[gnu::used]] static constexpr Style reset() {
    Style style;
    style._reset = true;
    return style;
};

static inline constexpr Style style(auto... args) {
    return Style{args...};
}

template <typename T>
struct Styled {
    T _inner;
    Style _color;
};

static inline auto styled(auto inner, Style style) {
    return Styled<decltype(inner)>{inner, style};
}

} // namespace Karm::Cli

template <>
struct Karm::Fmt::Formatter<Karm::Cli::Style> {
    Result<size_t> format(Io::_TextWriter &writer, Karm::Cli::Style style) {
#ifdef __osdk_karm_cli_backend_ansi__

        size_t written = 0;

        if (style._reset) {
            written += try$(writer.writeStr("\x1b[0m"));
        }

        if (style._fg != Karm::Cli::_COLOR_UNDEF) {
            written += try$(Fmt::format(writer, "\x1b[{}m", style._fg + 30));
        }

        if (style._bg != Karm::Cli::_COLOR_UNDEF) {
            written += try$(Fmt::format(writer, "\x1b[{}m", style._bg + 40));
        }

        if (style._bold) {
            written += try$(writer.writeStr("\x1b[1m"));
        }

        if (style._underline) {
            written += try$(writer.writeStr("\x1b[4m"));
        }

        if (style._blink) {
            written += try$(writer.writeStr("\x1b[5m"));
        }

        if (style._reverse) {
            written += try$(writer.writeStr("\x1b[7m"));
        }

        if (style._invisible) {
            written += try$(writer.writeStr("\x1b[8m"));
        }

        return written;
#else
        (void)writer;
        (void)style;
        return 0;
#endif
    }
};

template <typename T>
struct Karm::Fmt::Formatter<Karm::Cli::Styled<T>> {
    Formatter<Karm::Cli::Style> _styleFmt{};
    Formatter<T> _innerFmt{};

    void parse(Text::Scan &scan) {
        if constexpr (requires() {
                          _innerFmt.parse(scan);
                      }) {
            _innerFmt.parse(scan);
        }
    }

    Result<size_t> format(Io::_TextWriter &writer, Karm::Cli::Styled<T> const &styled) {
#ifdef __osdk_karm_cli_backend_ansi__
        return try$(_styleFmt.format(writer, styled._color)) +
               try$(_innerFmt.format(writer, styled._inner)) +
               try$(writer.writeStr("\x1b[0m"));
#else
        return _innerFmt.format(writer, styled._inner);
#endif
    }
};
