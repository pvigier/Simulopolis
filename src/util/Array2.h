/* Simulopolis
 * Copyright (C) 2018 Pierre Vigier
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

// STL
#include <vector>
// Boost
#include <boost/serialization/access.hpp>

/**
 * \brief 2D Array that stores the elements in 1D under the hood
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
     * \param height Number of rows
     * \param width Number of columns
     */
    Array2(std::size_t height = 0, std::size_t width = 0)
    {
        reshape(height, width);
    }

    /**
     * \brief Create a 2D array with specified size fiiled with a default value
     *
     * \param height Number of rows
     * \param width Number of columns
     * \param defaultValue Default value to fill the array with
     */
    Array2(std::size_t height, std::size_t width, const T& defaultValue)
    {
        reshape(height, width, defaultValue);
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
        return mData[i * mWidth + j];
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
        return mData[i * mWidth + j];
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
        mData[i * mWidth + j] = std::move(value);
    }

    /**
     * \brief Reshape the array
     *
     * \param height New height
     * \param width New width
     */
    inline void reshape(std::size_t height, std::size_t width)
    {
        mHeight = height;
        mWidth = width;
        mData.resize(mHeight * mWidth);
    }

    /**
     * \brief Reshape the array
     *
     * \param height New height
     * \param width New width
     */
    inline void reshape(std::size_t height, std::size_t width, const T& defaultValue)
    {
        mHeight = height;
        mWidth = width;
        mData.resize(mHeight * mWidth, defaultValue);
    }

    /**
     * \brief Return the number of elements in the array
     *
     * The size corresponds to mHeight*mWidth
     *
     * \return Number of elements in the array
     */
    inline std::size_t getSize() const
    {
        return mData.size();
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
     * \brief Return the width
     *
     * \return Width of the array
     */
    inline std::size_t getWidth() const
    {
        return mWidth;
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
    std::size_t mHeight; /**< Height or number of rows of the array */
    std::size_t mWidth; /**< Width or number of columns of the array */
    std::vector<T> mData; /**< std::vector where the elements are stored in */

    // Serialization
    friend class boost::serialization::access;

    template<typename Archive>
    void serialize(Archive& ar, const unsigned int /*version*/)
    {
        ar & mHeight & mWidth & mData;
    }
};

template<>
class Array2<bool>
{
public:
    Array2(std::size_t height = 0, std::size_t width = 0)
    {
        reshape(height, width);
    }

    Array2(std::size_t height, std::size_t width, const bool& defaultValue)
    {
        reshape(height, width, defaultValue);
    }

    ~Array2()
    {

    }

    inline bool get(std::size_t i, std::size_t j) const
    {
        return mData[i * mWidth + j];
    }

    inline void set(std::size_t i, std::size_t j, bool value)
    {
        mData[i * mWidth + j] = std::move(value);
    }

    inline void reshape(std::size_t height, std::size_t width)
    {
        mHeight = height;
        mWidth = width;
        mData.resize(mHeight * mWidth);
    }

    inline void reshape(std::size_t height, std::size_t width, const bool& defaultValue)
    {
        mHeight = height;
        mWidth = width;
        mData.resize(mHeight * mWidth, defaultValue);
    }

    inline std::size_t getSize() const
    {
        return mData.size();
    }

    inline std::size_t getHeight() const
    {
        return mHeight;
    }

    inline std::size_t getWidth() const
    {
        return mWidth;
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
    std::size_t mHeight;
    std::size_t mWidth;
    std::vector<bool> mData;

    // Serialization
    friend class boost::serialization::access;

    template<typename Archive>
    void serialize(Archive& ar, const unsigned int /*version*/)
    {
        ar & mHeight & mWidth & mData;
    }
};

