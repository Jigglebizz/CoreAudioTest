#pragma once

#include "services/speakerService/Device.h"

namespace objectModel {
   /**
    * Audio instruments and effects implement 'Renderable'
    */
    class Renderable {
    public:
      /**
       * The Buffer is passed down the chain of renderables,
       * and is ultimately what gets rendered to the device.
       *
       * A Renderable at the beginning of a chain can assume the
       * buffer is filled with garbage. Any other Renderable 
       * will operate on the buffer after the previous 
       * Renderable has finished operating on it.
       */
       virtual void render( float** Buffer, 
                            size_t NumChannels, 
                            size_t Length,
                            const speakerService::Device& Dev) noexcept = 0;
    };
}
