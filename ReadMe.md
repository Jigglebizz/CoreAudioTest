## Core Audio Test App
This application polls the system's audio devices and opens an audio stream to the default device.
There is a framework in place to build 'plugins' for generating or affecting audio.
Currently, a sine-wave generator and digital-noise generator are available, but I would 
like to build some effects like delay or reverb. :guitar:

Built and tested in Visual Studio Community 2017

### Intention
This is intended to be a sample of my coding ability. It is an incomplete project, 
and development is ongoing. However, because of this intention, I have included 
verbose comments to not only explain why I've done certain things, but also express 
my familiarity with various idioms in the C++ language as well. I do my best to 
highlight when I'm using a particular technique or pattern to demonstrate my 
experience. I may not otherwise employ such verbose comments - I believe that CODE 
should tell you what it does, if it is written well. Comments can become stale, 
and the only real way to couple functionality with communicating to the maintainer 
is to write *good code* :tm:

I wanted to build an application to interface with Windows Core Audio APIs
I considered this to be a decent challenge because
- Windows API Documentation is generally pretty poor, Core Audio is especially no exception :hankey:
- I haven't done a lot of "real" audio programming
- I want a playground to do some audio programming. I use Max-MSP for this normally, but I'd like it if what I build were portable to, say, an embedded system. :musical_note: :musical_note:

### Conventions
In some places, you will see a comment with a number in brackets
EG:

`// [1.]`

This is a footnote. I placed the comment at the bottom of the file to get a big, tangential comment out of the way.

### Where to start
To help with unit testing, all functionality outside of main() has been placed in a statically-linked lib. The App and Unit Tests are linked against this lib. There was a surprising amount of effort that went into making this work nicely with debug symbols, but it wasn't too difficult.

Much of the application's functionality is built around

`services/speakerService/Device.h`

This class is an adapter for the Windows APIs, and orchestrates the audio session.

To see how I handled Dependency Injection, check out Context.h