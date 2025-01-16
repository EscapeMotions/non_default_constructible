#pragma once

#include <iostream>
#include <functional>

// DefaultConstructible to be used just for consistent access to default and non-default variables via .get()
template<typename T>
class DefaultConstructible {
public:
    // Default constructor
    DefaultConstructible() = default;
    
    // Forwarding constructor for initialization with arguments
    template<typename... Args>
    DefaultConstructible(Args&&... args)
        : t_(std::forward<Args>(args)...) {}
    
    // Copy and move constructors and assignment operators
    DefaultConstructible(const DefaultConstructible&) = default;
    DefaultConstructible(DefaultConstructible&&) noexcept = default;
    DefaultConstructible& operator=(const DefaultConstructible&) = default;
    DefaultConstructible& operator=(DefaultConstructible&&) noexcept = default;
    
    // Access to T
    T& get() { return t_; }
    const T& get() const { return t_; }
    
private:
    T t_;
};

// Specialization for std::function
template<typename R, typename... Args>
class DefaultConstructible<std::function<R(Args...)>> {
public:
    DefaultConstructible() = default;

    template<typename F, 
             typename = std::enable_if_t<!std::is_same_v<std::decay_t<F>, DefaultConstructible>>>
    DefaultConstructible(F&& f)
        : func_(std::forward<F>(f)) {}

    DefaultConstructible(const DefaultConstructible&) = default;
    DefaultConstructible(DefaultConstructible&&) noexcept = default;
    DefaultConstructible& operator=(const DefaultConstructible&) = default;
    DefaultConstructible& operator=(DefaultConstructible&&) noexcept = default;

    // Provide std::function interface
    R operator()(Args... args) const {
        return func_(std::forward<Args>(args)...);
    }

    explicit operator bool() const noexcept {
        return static_cast<bool>(func_);
    }

    // Possibly add other std::function members as needed

private:
    std::function<R(Args...)> func_;
};



// General template - deleted default constructor
template<typename T>
class NonDefaultConstructible {
public:
    NonDefaultConstructible() = delete;

    template<typename... Args>
    NonDefaultConstructible(Args&&... args)
        : t_(std::forward<Args>(args)...) {}

    // Copy and move constructors and assignment operators
    NonDefaultConstructible(const NonDefaultConstructible&) = default;
    NonDefaultConstructible(NonDefaultConstructible&&) noexcept = default;
    NonDefaultConstructible& operator=(const NonDefaultConstructible&) = default;
    NonDefaultConstructible& operator=(NonDefaultConstructible&&) noexcept = default;

    // Access to T
    T& get() { return t_; }
    const T& get() const { return t_; }

private:
    T t_;
};

// Specialization for std::function
template<typename R, typename... Args>
class NonDefaultConstructible<std::function<R(Args...)>> {
public:
    typedef std::function<R(Args...)> Func;

    NonDefaultConstructible() = delete;

    template<typename F, 
             typename = std::enable_if_t<!std::is_same_v<std::decay_t<F>, NonDefaultConstructible>>>
    NonDefaultConstructible(F&& f)
        : func_(std::forward<F>(f)) {}

    NonDefaultConstructible(const NonDefaultConstructible&) = default;
    NonDefaultConstructible(NonDefaultConstructible&&) noexcept = default;
    NonDefaultConstructible& operator=(const NonDefaultConstructible&) = default;
    NonDefaultConstructible& operator=(NonDefaultConstructible&&) noexcept = default;

    // Access to T
    Func& get() { return func_; }
    const Func& get() const { return func_; }

    // Provide std::function interface
    R operator()(Args... args) const {
        return func_(std::forward<Args>(args)...);
    }

    explicit operator bool() const noexcept {
        return static_cast<bool>(func_);
    }

    // Possibly add other std::function members as needed

private:
    Func func_;
};
