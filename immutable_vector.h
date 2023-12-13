#pragma once
#include <vector>
#include <memory>

template <typename T>
class Node {
public:
    Node() {
    }

    Node(const T& value) : value_(value) {
    }

    const T& Get(int index) const {
        if (children_.empty()) {
            return value_;
        }
        return children_[GetChild(index)]->Get(PassToChild(index));
    }

    std::shared_ptr<Node<T>> Set(size_t index, const T& value) const {
        if (children_.empty() && index == 0) {
            return std::make_shared<Node<T>>(value);
        }
        auto ret = std::make_shared<Node<T>>();
        ret->children_ = children_;
        if (children_.empty()) {
            ret->children_.resize(1ul << kBits);
            ret->children_[0] = std::make_shared<Node<T>>(value_);
        }
        if (ret->children_[GetChild(index)] == nullptr) {
            ret->children_[GetChild(index)] = std::make_shared<Node<T>>(value);
            return ret;
        }
        ret->children_[GetChild(index)] =
                ret->children_[GetChild(index)]->Set(PassToChild(index), value);
        return ret;
    }

private:
    static const size_t kBits;

    T value_;
    std::vector<std::shared_ptr<Node<T>>> children_;

    size_t GetChild(size_t index) const {
        return index & ((1ul << kBits) - 1);
    }

    size_t PassToChild(size_t index) const {
        return index >> kBits;
    }
};

template <typename T>
const size_t Node<T>::kBits = 4;

template <class T>
class ImmutableVector {
public:
    ImmutableVector() : size_(0), root_(std::make_shared<Node<T>>()) {
    }

    explicit ImmutableVector(size_t count, const T& value = T()) : ImmutableVector() {
        for (size_t i = 0; i < count; ++i) {
            root_ = root_->Set(size_, value);
            ++size_;
        }
    }

    template <typename Iterator>
    ImmutableVector(Iterator first, Iterator last) : ImmutableVector() {
        while (first != last) {
            root_ = root_->Set(size_, *first);
            ++size_;
            ++first;
        }
    }

    ImmutableVector(std::initializer_list<T> l) : ImmutableVector() {
        for (const auto& x : l) {
            root_ = root_->Set(size_, x);
            ++size_;
        }
    }

    ImmutableVector Set(size_t index, const T& value) {
        return ImmutableVector(size_, root_->Set(index, value));
    }

    const T& Get(size_t index) const {
        return root_->Get(index);
    }

    ImmutableVector PushBack(const T& value) {
        return ImmutableVector(size_ + 1, root_->Set(size_, value));
    }

    ImmutableVector PopBack() {
        return ImmutableVector(size_ - 1, root_);
    }

    size_t Size() const {
        return size_;
    }

private:
    ImmutableVector(size_t sz, std::shared_ptr<Node<T>> rt) : size_(sz), root_(rt) {
    }

    std::shared_ptr<Node<T>> root_;
    size_t size_;
};
