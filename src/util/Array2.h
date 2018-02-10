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
    Array2(const std::size_t width, const std::size_t height) : mWidth(width), mHeight(height)
    {
        mData.resize(mWidth * mHeight);
    }

    /**
     * \brief Create a 2D array with specified size fiiled with a default value
     *
     * \param width Number of rows
     * \param height Number of columns
     * \param defaultValue Default value to fill the array with
     */
    Array2(const std::size_t width, const std::size_t height, const T defaultValue) :
        mWidth(width), mHeight(height)
    {
        mData.resize(mWidth * mHeight, defaultValue);
    }

    /**
     * \brief Destructor
     */
    ~Array2()
    {

    }

    /**
     * \brief Get an element
     *
     * The element is passed by value not by reference.
     *
     * \param i First coordinate of the element
     * \param j Second coordinate of the element
     *
     * \return Element at position (i, j)
     */
    inline T get(const std::size_t i, const std::size_t j) const
    {
        return mData[i * mHeight + j];
    }

    /**
     *\brief Set an element
     *
     * The element is copied in the array.
     *
     * \param i First coordinate of the element
     * \param j Second coordinate of the element
     * \param value Value to copy in the array
     */
    inline void set(const std::size_t i, const std::size_t j, const T value)
    {
        mData[i * mHeight + j] = value;
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
