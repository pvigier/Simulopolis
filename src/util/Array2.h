#pragma once

// STL
#include <iostream>
#include <vector>

/**
 * \brief 2D Array that stores the elements in 1D under the hood
 *
 * There are two notations for the coordinates in a 2D array:
 * \li matrix notation (i, j): the size corresponds to the number of
 * rows and the number of columns.
 * \li cartesian notation (x, y): the size corresponds to the width and
 * the height.
 *
 * We mix the two notations here.
 *
 * \author Pierre Vigier
 */
template<typename T>
class Array2
{
public:
    /**
     * \brief Create a 2D array with specified size
     *
     * \param width Number of rows
     * \param height Number of columns
     */
    Array2(std::size_t width = 0, std::size_t height = 0)
    {
        reshape(width, height);
    }

    /**
     * \brief Create a 2D array with specified size fiiled with a default value
     *
     * \param width Number of rows
     * \param height Number of columns
     * \param defaultValue Default value to fill the array with
     */
    Array2(std::size_t width, std::size_t height, const T& defaultValue)
    {
        reshape(width, height, defaultValue);
    }

    /**
     * \brief Destructor
     */
    ~Array2()
    {

    }

    /**
     * \brief Get reference to an element
     *
     * The element is passed by value not by reference.
     *
     * \param i First coordinate of the element
     * \param j Second coordinate of the element
     *
     * \return Reference to element at position (i, j)
     */
    inline T& get(std::size_t i, std::size_t j)
    {
        return mData[i * mHeight + j];
    }

    /**
     * \brief Get const reference to an element
     *
     * The element is passed by value not by reference.
     *
     * \param i First coordinate of the element
     * \param j Second coordinate of the element
     *
     * \return Const reference to element at position (i, j)
     */
    inline const T& get(std::size_t i, std::size_t j) const
    {
        return mData[i * mHeight + j];
    }

    /**
     * \brief Set an element
     *
     * The element is copied in the array.
     *
     * \param i First coordinate of the element
     * \param j Second coordinate of the element
     * \param value Value to copy in the array
     */
    inline void set(std::size_t i, std::size_t j, T value)
    {
        mData[i * mHeight + j] = std::move(value);
    }

    /**
     * \brief Reshape the array
     *
     * \param width New width
     * \param height New height
     */
    inline void reshape(std::size_t width, std::size_t height)
    {
        mWidth = width;
        mHeight = height;
        mData.resize(mWidth * mHeight);
    }

    /**
     * \brief Reshape the array
     *
     * \param width New width
     * \param height New height
     */
    inline void reshape(std::size_t width, std::size_t height, const T& defaultValue)
    {
        mWidth = width;
        mHeight = height;
        mData.resize(mWidth * mHeight, defaultValue);
    }

    /**
     * \brief Return the number of elements in the array
     *
     * The size corresponds to mWidth*mHeight
     *
     * \return Number of elements in the array
     */
    inline std::size_t getSize() const
    {
        return mData.size();
    }

    /**
     * \brief Return the width
     *
     * \return Width of the array
     */
    inline std::size_t getWidth() const
    {
        return mWidth;
    }

    /**
     * \brief Return the height
     *
     * \return Height of the array
     */
    inline std::size_t getHeight() const
    {
        return mHeight;
    }

    /**
     * \brief Return the elements
     *
     * \return Reference on the vector where the elements are stored in
     */
    std::vector<T>& getData()
    {
        return mData;
    }

    /**
     * \brief Return the elements
     *
     * \return Const reference on the std::vector where the elements are stored in
     */
    const std::vector<T>& getData() const
    {
        return mData;
    }

private:
    std::size_t mWidth; /**< Width or number of rows of the array */
    std::size_t mHeight; /**< Height or number of columns of the array */
    std::vector<T> mData; /**< std::vector where the elements are stored in */
};

template<>
class Array2<bool>
{
public:
    Array2(std::size_t width = 0, std::size_t height = 0)
    {
        reshape(width, height);
    }

    Array2(std::size_t width, std::size_t height, const bool& defaultValue)
    {
        reshape(width, height, defaultValue);
    }

    ~Array2()
    {

    }

    inline bool get(std::size_t i, std::size_t j) const
    {
        return mData[i * mHeight + j];
    }

    inline void set(std::size_t i, std::size_t j, bool value)
    {
        mData[i * mHeight + j] = std::move(value);
    }

    inline void reshape(std::size_t width, std::size_t height)
    {
        mWidth = width;
        mHeight = height;
        mData.resize(mWidth * mHeight);
    }

    inline void reshape(std::size_t width, std::size_t height, const bool& defaultValue)
    {
        mWidth = width;
        mHeight = height;
        mData.resize(mWidth * mHeight, defaultValue);
    }

    inline std::size_t getSize() const
    {
        return mData.size();
    }

    inline std::size_t getWidth() const
    {
        return mWidth;
    }

    inline std::size_t getHeight() const
    {
        return mHeight;
    }

    std::vector<bool>& getData()
    {
        return mData;
    }

    const std::vector<bool>& getData() const
    {
        return mData;
    }

private:
    std::size_t mWidth;
    std::size_t mHeight;
    std::vector<bool> mData;
};

