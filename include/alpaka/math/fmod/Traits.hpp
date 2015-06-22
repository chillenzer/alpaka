/**
* \file
* Copyright 2014-2015 Benjamin Worpitz
*
* This file is part of alpaka.
*
* alpaka is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* alpaka is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with alpaka.
* If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <alpaka/core/Common.hpp>   // ALPAKA_FCT_HOST_ACC

#include <type_traits>              // std::enable_if, std::is_base_of, std::is_same, std::decay

namespace alpaka
{
    namespace math
    {
        namespace traits
        {
            //#############################################################################
            //! The fmod trait.
            //#############################################################################
            template<
                typename T,
                typename Tx,
                typename Ty,
                typename TSfinae = void>
            struct Fmod;
        }

        //-----------------------------------------------------------------------------
        //! Computes the floating-point remainder of the division operation x/y.
        //!
        //! \tparam T The type of the object specializing Fmod.
        //! \tparam Tx The type of the first argument.
        //! \tparam Ty The type of the second argument.
        //! \param fmod The object specializing Fmod.
        //! \param x The first argument.
        //! \param y The second argument.
        //-----------------------------------------------------------------------------
        template<
            typename T,
            typename Tx,
            typename Ty>
        ALPAKA_FCT_HOST_ACC auto fmod(
            T const & fmod,
            Tx const & x,
            Ty const & y)
        -> decltype(
            traits::Fmod<
                T,
                Tx,
                Ty>
            ::fmod(
                fmod,
                x,
                y))
        {
            return traits::Fmod<
                T,
                Tx,
                Ty>
            ::fmod(
                fmod,
                x,
                y);
        }

        namespace traits
        {
            //#############################################################################
            //! The Fmod specialization for classes with FmodBase member type.
            //#############################################################################
            template<
                typename T,
                typename TArg>
            struct Fmod<
                T,
                TArg,
                typename std::enable_if<
                    std::is_base_of<typename T::FmodBase, typename std::decay<T>::type>::value
                    && (!std::is_same<typename T::FmodBase, typename std::decay<T>::type>::value)>::type>
            {
                //-----------------------------------------------------------------------------
                //
                //-----------------------------------------------------------------------------
                ALPAKA_FCT_HOST_ACC static auto fmod(
                    T const & fmod,
                    TArg const & arg)
                -> decltype(
                    math::fmod(
                        static_cast<typename T::FmodBase const &>(fmod),
                        arg))
                {
                    // Delegate the call to the base class.
                    return
                        math::fmod(
                            static_cast<typename T::FmodBase const &>(fmod),
                            arg);
                }
            };
        }
    }
}
