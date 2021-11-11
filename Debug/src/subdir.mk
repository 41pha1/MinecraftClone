################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Block.cpp \
../src/BlockShader.cpp \
../src/BoundingBoxRenderer.cpp \
../src/BoundingBoxShader.cpp \
../src/Camera.cpp \
../src/Chunk.cpp \
../src/ChunkProvider.cpp \
../src/ChunkRenderer.cpp \
../src/Game.cpp \
../src/ImageLoader.cpp \
../src/Input.cpp \
../src/MasterRenderer.cpp \
../src/MinecraftClone.cpp \
../src/Mouse.cpp \
../src/Player.cpp \
../src/ShaderProgram.cpp \
../src/Simplex.cpp \
../src/Util.cpp \
../src/VAO.cpp \
../src/VAOLoader.cpp \
../src/WorldGenerator.cpp \
../src/lodepng.cpp 

C_SRCS += \
../src/glad.c 

C_DEPS += \
./src/glad.d 

OBJS += \
./src/Block.o \
./src/BlockShader.o \
./src/BoundingBoxRenderer.o \
./src/BoundingBoxShader.o \
./src/Camera.o \
./src/Chunk.o \
./src/ChunkProvider.o \
./src/ChunkRenderer.o \
./src/Game.o \
./src/ImageLoader.o \
./src/Input.o \
./src/MasterRenderer.o \
./src/MinecraftClone.o \
./src/Mouse.o \
./src/Player.o \
./src/ShaderProgram.o \
./src/Simplex.o \
./src/Util.o \
./src/VAO.o \
./src/VAOLoader.o \
./src/WorldGenerator.o \
./src/glad.o \
./src/lodepng.o 

CPP_DEPS += \
./src/Block.d \
./src/BlockShader.d \
./src/BoundingBoxRenderer.d \
./src/BoundingBoxShader.d \
./src/Camera.d \
./src/Chunk.d \
./src/ChunkProvider.d \
./src/ChunkRenderer.d \
./src/Game.d \
./src/ImageLoader.d \
./src/Input.d \
./src/MasterRenderer.d \
./src/MinecraftClone.d \
./src/Mouse.d \
./src/Player.d \
./src/ShaderProgram.d \
./src/Simplex.d \
./src/Util.d \
./src/VAO.d \
./src/VAOLoader.d \
./src/WorldGenerator.d \
./src/lodepng.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"C:\Users\Janny8\Documents\workspace\MinecraftClone\libs\include" -I"C:\MinGW\include\SDL2" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/Camera.o: ../src/Camera.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"src/Camera.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"C:\Users\Janny8\Documents\workspace\MinecraftClone\libs\include" -I"C:\MinGW\include\SDL2" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


