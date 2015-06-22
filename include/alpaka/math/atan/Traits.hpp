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
            //! The atan trait.
            //#############################################################################
            template<
                typename T,
                typename TArg,
                typename TSfinae = void>
            struct Atan;
        }

        //-----------------------------------------------------------------------------
        //! Computes the principal value of the arc tangent.
        //!
        //! \tparam TArg The arg type.
        //! \param value The value.
        //-----------------------------------------------------------------------------
        template<
            typename T,
            typename TArg>
        ALPAKA_FCT_HOST_ACC auto atan(
            T const & atan,
            TArg const & arg)
        -> decltype(
            traits::Atan<
                T,
                TArg>
            ::atan(
                atan,
                arg))
        {
            return traits::Atan<
                T,
                TArg>
            ::atan(
                atan,
                arg);
        }

        namespace traits
        {
            //#############################################################################
            //! The Atan specialization for classes with AtanBase member type.
            //#############################################################################
            template<
                typename T,
                typename TArg>
            struct Atan<
                T,
                TArg,
                typename std::enable_if<
                    std::is_base_of<typename T::AtanBase, typename std::decay<T>::type>::value
                    && (!std::is_same<typename T::AtanBase, typename std::decay<T>::type>::value)>::type>
            {
                //-----------------------------------------------------------------------------
                //
                //-----------------------------------------------------------------------------
                ALPAKA_FCT_HOST_ACC static auto atan(
                    T const & atan,
                    TArg const & arg)
                -> decltype(
                    math::atan(
                        static_cast<typename T::AtanBase const &>(atan),
                        arg))
                {
                    // Delegate the call to the base class.
                    return
                        math::atan(
                            static_cast<typename T::AtanBase const &>(atan),
                            arg);
                }
            };
        }
    }
}
