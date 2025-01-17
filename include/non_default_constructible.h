#pragma once

#include <iostream>
#include <string>
#include <functional>
#include <type_traits>
#include <utility>

// NonDefaultConstructible: disable the default constructor of the wrapped type
template<typename T>
class NonDefaultConstructible {
public:
    // Disable the default constructor
    NonDefaultConstructible() = delete;

    // Copy and move constructors
    NonDefaultConstructible(const NonDefaultConstructible&) = default;
    NonDefaultConstructible(NonDefaultConstructible&&) noexcept = default;
    NonDefaultConstructible& operator=(const NonDefaultConstructible&) = default;
    NonDefaultConstructible& operator=(NonDefaultConstructible&&) noexcept = default;

    // Template constructor with SFINAE to prevent ambiguity with copy/move constructors
    // Checks if none of the argument types decay to `NonDefaultConstructible` (or `DefaultConstructible`),
    // preventing the template constructor from being considered during copy/move construction.
    template<typename... Args,
        typename = std::enable_if_t<
            ( !std::is_same_v<NonDefaultConstructible<T>, std::decay_t<Args>> && ... )
    >>
    NonDefaultConstructible(Args&&... args)
        : t_(std::forward<Args>(args)...) {}


    // Access to T
    T& get() { return t_; }
    const T& get() const { return t_; }

    // operator() when T is callable
    template<typename... Args>
    auto operator()(Args&&... args) const
        -> std::enable_if_t<std::is_invocable_v<const T&, Args&&...>,
                            decltype(std::invoke(std::declval<const T&>(), std::forward<Args>(args)...))>
    {
        return std::invoke(t_, std::forward<Args>(args)...);
    }

private:
    T t_;
};

// DefaultConstructible to be used just for consistent access to default and non-default variables via .get()
template<typename T>
class DefaultConstructible {
public:
    // Default constructor
    DefaultConstructible() = default;

    // Copy and move constructors
    DefaultConstructible(const DefaultConstructible&) = default;
    DefaultConstructible(DefaultConstructible&&) noexcept = default;
    DefaultConstructible& operator=(const DefaultConstructible&) = default;
    DefaultConstructible& operator=(DefaultConstructible&&) noexcept = default;

    // Template constructor with SFINAE
    template<typename... Args,
        typename = std::enable_if_t<
            ( !std::is_same_v<DefaultConstructible<T>, std::decay_t<Args>> && ... )
    >>
    DefaultConstructible(Args&&... args)
        : t_(std::forward<Args>(args)...) {}

    // Access to T
    T& get() { return t_; }
    const T& get() const { return t_; }

    // operator() when T is callable
    template<typename... Args>
    auto operator()(Args&&... args) const
        -> std::enable_if_t<std::is_invocable_v<const T&, Args&&...>,
                            decltype(std::invoke(std::declval<const T&>(), std::forward<Args>(args)...))>
    {
        return std::invoke(t_, std::forward<Args>(args)...);
    }

private:
    T t_;
};
