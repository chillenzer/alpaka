/**
* Copyright 2014 Benjamin Worpitz
*
* This file is part of alpaka.
*
* alpaka is free software: you can redistribute it and/or modify
* it under the terms of either the GNU General Public License or
* the GNU Lesser General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* alpaka is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License and the GNU Lesser General Public License
* for more details.
*
* You should have received a copy of the GNU General Public License
* and the GNU Lesser General Public License along with alpaka.
* If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <alpaka/fibers/AccFibersFwd.hpp>   // AccFibers

#include <alpaka/host/SystemInfo.hpp>       // host::getCpuName, host::getGlobalMemorySizeBytes

#include <alpaka/interfaces/Device.hpp>     // alpaka::device::Device, alpaka::device::DeviceManager

#include <sstream>                          // std::stringstream
#include <limits>                           // std::numeric_limits
#include <thread>                           // std::thread

namespace alpaka
{
    namespace cuda
    {
        namespace detail
        {
            // Forward declaration.
            class DeviceManagerFibers;

            //#############################################################################
            //! The CUDA accelerator device handle.
            //#############################################################################
            class DeviceFibers
            {
                friend class DeviceManagerFibers;
            protected:
                //-----------------------------------------------------------------------------
                //! Constructor.
                //-----------------------------------------------------------------------------
                ALPAKA_FCT_HOST DeviceFibers() = default;
            public:
                //-----------------------------------------------------------------------------
                //! Copy-constructor.
                //-----------------------------------------------------------------------------
                ALPAKA_FCT_HOST DeviceFibers(DeviceFibers const &) = default;
                //-----------------------------------------------------------------------------
                //! Move-constructor.
                //-----------------------------------------------------------------------------
                ALPAKA_FCT_HOST DeviceFibers(DeviceFibers &&) = default;
                //-----------------------------------------------------------------------------
                //! Assignment-operator.
                //-----------------------------------------------------------------------------
                ALPAKA_FCT_HOST DeviceFibers & operator=(DeviceFibers const &) = default;
                //-----------------------------------------------------------------------------
                //! Destructor.
                //-----------------------------------------------------------------------------
                ALPAKA_FCT_HOST ~DeviceFibers() noexcept = default;

            protected:
                //-----------------------------------------------------------------------------
                //! \return The device properties.
                //-----------------------------------------------------------------------------
                ALPAKA_FCT_HOST device::DeviceProperties getProperties() const
                {
                    device::DeviceProperties deviceProperties;

                    deviceProperties.m_sName = host::getCpuName();
                    deviceProperties.m_uiBlockKernelSizeMax = 1024; // FIXME: What is the maximum? Just set a reasonable value?
                    deviceProperties.m_v3uiBlockKernelSizePerDimMax = vec<3u>(deviceProperties.m_uiBlockKernelSizeMax, deviceProperties.m_uiBlockKernelSizeMax, deviceProperties.m_uiBlockKernelSizeMax);
                    deviceProperties.m_v3uiGridBlockSizePerDimMax = vec<3u>(std::numeric_limits<std::size_t>::max(), std::numeric_limits<std::size_t>::max(), std::numeric_limits<std::size_t>::max());
                    deviceProperties.m_uiExecutionUnitCount = std::thread::hardware_concurrency();          // TODO: This may be inaccurate. // TODO: Would it be better to not rely on std::thread? But we are already requiring c++11 so std::thread is always available.
                    deviceProperties.m_uiGlobalMemorySizeBytes = host::getGlobalMemorySizeBytes();
                    //deviceProperties.m_uiClockFrequencyHz = TODO;

                    return deviceProperties;
                }
            };
        }
    }

    namespace device
    {
        //#############################################################################
        //! The CUDA accelerator device handle.
        //#############################################################################
        template<>
        class Device<AccFibers> :
            public device::detail::IDevice<cuda::detail::DeviceFibers>
        {
            friend class cuda::detail::DeviceManagerFibers;
        private:
            //-----------------------------------------------------------------------------
            //! Constructor.
            //-----------------------------------------------------------------------------
            ALPAKA_FCT_HOST Device() = default;

        public:
            //-----------------------------------------------------------------------------
            //! Copy-constructor.
            //-----------------------------------------------------------------------------
            ALPAKA_FCT_HOST Device(Device const &) = default;
            //-----------------------------------------------------------------------------
            //! Move-constructor.
            //-----------------------------------------------------------------------------
            ALPAKA_FCT_HOST Device(Device &&) = default;
            //-----------------------------------------------------------------------------
            //! Assignment-operator.
            //-----------------------------------------------------------------------------
            ALPAKA_FCT_HOST Device & operator=(Device const &) = default;
            //-----------------------------------------------------------------------------
            //! Destructor.
            //-----------------------------------------------------------------------------
            ALPAKA_FCT_HOST ~Device() noexcept = default;
        };
    }

    namespace cuda
    {
        namespace detail
        {
            //#############################################################################
            //! The CUDA accelerator device manager.
            //#############################################################################
            class DeviceManagerFibers
            {
            public:
                //-----------------------------------------------------------------------------
                //! Constructor.
                //-----------------------------------------------------------------------------
                ALPAKA_FCT_HOST DeviceManagerFibers() = delete;

                //-----------------------------------------------------------------------------
                //! \return The number of devices available.
                //-----------------------------------------------------------------------------
                ALPAKA_FCT_HOST static std::size_t getDeviceCount()
                {
                    return 1;
                }
                //-----------------------------------------------------------------------------
                //! \return The number of devices available.
                //-----------------------------------------------------------------------------
                ALPAKA_FCT_HOST static device::Device<AccFibers> getDeviceByIndex(std::size_t const & uiIndex)
                {
                    std::size_t const uiNumDevices(getDeviceCount());
                    if(uiIndex >= uiNumDevices)
                    {
                        std::stringstream ssErr;
                        ssErr << "Unable to return device handle for device " << uiIndex << " because there are only " << uiNumDevices << " threads devices!";
                        throw std::runtime_error(ssErr.str());
                    }

                    return {};
                }
                //-----------------------------------------------------------------------------
                //! \return The handle to the currently used device.
                //-----------------------------------------------------------------------------
                ALPAKA_FCT_HOST static device::Device<AccFibers> getCurrentDevice()
                {
                    return {};
                }
                //-----------------------------------------------------------------------------
                //! Sets the device to use with this accelerator.
                //-----------------------------------------------------------------------------
                ALPAKA_FCT_HOST static void setCurrentDevice(device::Device<AccFibers> const & )
                {
                    // The code is already running on this device.
                }
            };
        }
    }

    namespace device
    {
        //#############################################################################
        //! The threads accelerator device manager.
        //#############################################################################
        template<>
        class DeviceManager<AccFibers> :
            public detail::IDeviceManager<cuda::detail::DeviceManagerFibers>
        {};
    }
}
