#ifndef HAL_FLASH_INTERNAL_STM_DETAIL_HPP
#define HAL_FLASH_INTERNAL_STM_DETAIL_HPP

#include DEVICE_HEADER
#include "services/util/FlashAlign.hpp"
#include <cstdint>

namespace hal
{
    namespace detail
    {
        template<typename alignment>
        void AlignedWriteBufferByAddress(services::FlashAlign::Chunk& chunk, uint32_t& fullAddress, uint32_t flashType)
        {
            auto range = infra::ReinterpretCastMemoryRange<const alignment>(chunk.data);
            for (const auto& data : range)
            {
                auto result = HAL_FLASH_Program(flashType, fullAddress, reinterpret_cast<uint32_t>(&data));
                really_assert(result == HAL_OK);
                fullAddress += sizeof(alignment);
            }
        }

        template<typename alignment>
        void AlignedWriteBufferByValue(services::FlashAlign::Chunk& chunk, uint32_t& fullAddress, uint32_t flashType)
        {
            auto range = infra::ReinterpretCastMemoryRange<const alignment>(chunk.data);
            for (alignment data : range)
            {
                auto result = HAL_FLASH_Program(flashType, fullAddress, data);
                really_assert(result == HAL_OK);
                fullAddress += sizeof(alignment);
            }
        }

        template<typename alignment, uint32_t flashType, bool byAddress>
        void AlignedWriteBuffer(infra::ConstByteRange buffer, uint32_t address, uint32_t flashMemoryBegin)
        {
            services::FlashAlign::WithAlignment<sizeof(alignment)> flashAlign;
            flashAlign.Align(address, buffer);

            services::FlashAlign::Chunk* chunk = flashAlign.First();
            while (chunk != nullptr)
            {
                really_assert(chunk->data.size() % sizeof(alignment) == 0);
                auto fullAddress = flashMemoryBegin + chunk->alignedAddress;

                if constexpr (byAddress)
                    detail::AlignedWriteBufferByAddress<alignment>(*chunk, fullAddress, flashType);
                else
                    detail::AlignedWriteBufferByValue<alignment>(*chunk, fullAddress, flashType);

                chunk = flashAlign.Next();
            }
        }
    }
}

#endif
