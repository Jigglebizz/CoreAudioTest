#pragma once

#include "services/speakerService/Device.h"

namespace objectModel {
   /**
    * Audio instruments and effects implement 'Renderable'
    */
    class Renderable {
    public:
      /**
       * The Buffer is what gets rendered to the device.
       * A Renderable at the beginning of a chain will have a
       * blank buffer. A Renderable in the middle of a chain will
       * recieve the buffer from the previous Renderable in the chain.
       */
       virtual void render( float** Buffer, 
                            size_t NumChannels, 
                            size_t Length,
                            const speakerService::Device& Dev) noexcept = 0;
    };
}
