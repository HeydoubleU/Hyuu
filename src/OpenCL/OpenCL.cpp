#include "OpenCL.h"

#include <Amino/Cpp/ClassDefine.h>

AMINO_DEFINE_DEFAULT_CLASS(Hyuu::OpenCL::Memory::Buffer);
AMINO_DEFINE_DEFAULT_CLASS(Hyuu::OpenCL::Memory::LocalBuffer);
AMINO_DEFINE_DEFAULT_CLASS(Hyuu::OpenCL::Memory::Image::Image1DBuffer);
AMINO_DEFINE_DEFAULT_CLASS(Hyuu::OpenCL::Memory::Image::Image2DBuffer);
AMINO_DEFINE_DEFAULT_CLASS(Hyuu::OpenCL::Memory::Image::Image3DBuffer);
AMINO_DEFINE_DEFAULT_CLASS(Hyuu::OpenCL::Execute::Program);
AMINO_DEFINE_DEFAULT_CLASS(Hyuu::OpenCL::Execute::Kernel);

#define CL_INIT_CHECK() if (!initOpenCL()) { return; }


int CL_DEVICE_ID = -1;  // if < 0, cl has not yet or failed to be initialized
cl::Device CL_DEVICE;
cl::Context CL_CONTEXT;
cl::CommandQueue CL_QUEUE;

cl::Context& getContext() { return CL_CONTEXT; }
cl::CommandQueue& getQueue() { return CL_QUEUE; }

std::vector<cl::Device> listDevices() {
	std::vector<cl::Platform> _platforms;
	cl::Platform::get(&_platforms);
	std::vector<cl::Device> devices;

	for (auto& platform : _platforms) {
		std::vector<cl::Device> _devices;
		platform.getDevices(CL_DEVICE_TYPE_GPU, &_devices);
		devices.insert(devices.end(), _devices.begin(), _devices.end());
	}

	for (auto& platform : _platforms) {
		std::vector<cl::Device> _devices;
		platform.getDevices(CL_DEVICE_TYPE_CPU, &_devices);
		devices.insert(devices.end(), _devices.begin(), _devices.end());
	}

	return devices;
}


int desiredDeviceId() {
	const char* device_id_env = std::getenv("HYUU_OPENCL_DEVICE_ID");
	if (device_id_env) {
		try {
			return std::stoi(device_id_env);
		}
		catch (std::invalid_argument& e) {
			return 0;
		}
	}
	else
		return 0;
}


int initOpenCL() {
	if (CL_DEVICE_ID >= 0) {
		return 1;
	}

	auto devices = listDevices();
	if (devices.empty()) {
		return 0;
	}

	int device_id = desiredDeviceId();
	CL_DEVICE_ID = std::min(device_id, (int)devices.size() - 1);
	CL_DEVICE = devices[CL_DEVICE_ID];

	// Create OpenCL context and command queue
	CL_CONTEXT = cl::Context(CL_DEVICE);
	CL_QUEUE = cl::CommandQueue(CL_CONTEXT, CL_DEVICE);

	return 1;

}


// Utility =============================================================================================================

// disabled, see header.
//bool Hyuu::OpenCL::Utilitiy::opencl_restart() {
//	auto devices = listDevices();
//	if (devices.empty()) {
//		return false;
//	}
//
//	int device_id = desiredDeviceId();
//	CL_DEVICE_ID = std::min(device_id, (int)devices.size() - 1);
//	CL_DEVICE = devices[CL_DEVICE_ID];
//
//	// Create OpenCL context and command queue
//	CL_CONTEXT = cl::Context(CL_DEVICE);
//	CL_QUEUE = cl::CommandQueue(CL_CONTEXT, CL_DEVICE);
//
//	return true;
//}


void Hyuu::OpenCL::Utilitiy::opencl_device_info(Amino::MutablePtr<Bifrost::Object>& device_info)
{
	device_info = Bifrost::createObject();

	auto _info = Array<Amino::String>();
	auto devices = listDevices();
	int desired_id = desiredDeviceId();
	int actual_id = std::min(desired_id, (int)devices.size() - 1);

	for (int i = 0; i < devices.size(); i++) {
		auto label = Amino::String(devices[i].getInfo<CL_DEVICE_NAME>());
		if (i == actual_id) {
			label += " (active)";
		}
		_info.push_back(label);
	}

	Amino::String env_var = Amino::String("HYUU_OPENCL_DEVICE_ID = ") + Amino::String(std::to_string(desired_id));
	device_info->setProperty("devices", Amino::newClassPtr<Array<Amino::String>>(std::move(_info)));
	device_info->setProperty("env_var", env_var);

	CL_INIT_CHECK();

	cl::size_type maxWorkGroupSize;
	cl_ulong maxLocalMemorySize;
	cl_ulong globalMemorySize;
	cl_uint maxComputeUnits;
	cl::size_type maxWorkItemDimensions;
	std::vector<cl::size_type> maxWorkItemSizes; Array<Amino::ulong_t> _maxWorkItemSizes;
	cl_ulong maxMemAllocSize;
	cl_ulong maxConstantBufferSize;
	cl_uint maxConstantArgs;
	cl_uint maxSamplerArgs;
	cl_uint maxReadImageArgs;
	cl_uint maxWriteImageArgs;
	cl_bool imageSupport; Amino::bool_t _imageSupport;
	std::string deviceName;
	cl_device_local_mem_type localMemType;


	CL_DEVICE.getInfo(CL_DEVICE_MAX_WORK_GROUP_SIZE, &maxWorkGroupSize);
	CL_DEVICE.getInfo(CL_DEVICE_LOCAL_MEM_SIZE, &maxLocalMemorySize);
	CL_DEVICE.getInfo(CL_DEVICE_GLOBAL_MEM_SIZE, &globalMemorySize);
	CL_DEVICE.getInfo(CL_DEVICE_MAX_COMPUTE_UNITS, &maxComputeUnits);
	CL_DEVICE.getInfo(CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, &maxWorkItemDimensions);
	CL_DEVICE.getInfo(CL_DEVICE_MAX_WORK_ITEM_SIZES, &maxWorkItemSizes); for (auto& size : maxWorkItemSizes) { _maxWorkItemSizes.push_back(size); }
	CL_DEVICE.getInfo(CL_DEVICE_MAX_MEM_ALLOC_SIZE, &maxMemAllocSize);
	CL_DEVICE.getInfo(CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE, &maxConstantBufferSize);
	CL_DEVICE.getInfo(CL_DEVICE_MAX_CONSTANT_ARGS, &maxConstantArgs);
	CL_DEVICE.getInfo(CL_DEVICE_MAX_SAMPLERS, &maxSamplerArgs);
	CL_DEVICE.getInfo(CL_DEVICE_MAX_READ_IMAGE_ARGS, &maxReadImageArgs);
	CL_DEVICE.getInfo(CL_DEVICE_MAX_WRITE_IMAGE_ARGS, &maxWriteImageArgs);
	CL_DEVICE.getInfo(CL_DEVICE_IMAGE_SUPPORT, &imageSupport); _imageSupport = imageSupport;
	CL_DEVICE.getInfo(CL_DEVICE_NAME, &deviceName);

	auto device_specs = Bifrost::createObject();
	device_specs->setProperty("max_work_group_size", maxWorkGroupSize);
	device_specs->setProperty("max_local_memory_size", maxLocalMemorySize);
	device_specs->setProperty("global_memory_size", globalMemorySize);
	device_specs->setProperty("max_compute_units", maxComputeUnits);
	device_specs->setProperty("max_work_item_dimensions", maxWorkItemDimensions);
	device_specs->setProperty("max_work_item_sizes", Amino::newClassPtr<Array<Amino::ulong_t>>(std::move(_maxWorkItemSizes)));
	device_specs->setProperty("max_mem_alloc_size", maxMemAllocSize);
	device_specs->setProperty("max_constant_buffer_size", maxConstantBufferSize);
	device_specs->setProperty("max_constant_args", maxConstantArgs);
	device_specs->setProperty("max_samplers", maxSamplerArgs);
	device_specs->setProperty("max_read_image_args", maxReadImageArgs);
	device_specs->setProperty("max_write_image_args", maxWriteImageArgs);
	device_specs->setProperty("image_support", _imageSupport);
	device_specs->setProperty("device_name", Amino::String(deviceName));

	device_info->setProperty("device_specs", device_specs.toImmutable());
}

Amino::String formatToString(cl::ImageFormat format) {
	Amino::String channel_order;
	Amino::String channel_data_type;

	switch (format.image_channel_order) {
	case CL_R: channel_order = "CL_R"; break;
	case CL_A: channel_order = "CL_A"; break;
	case CL_RG: channel_order = "CL_RG"; break;
	case CL_RA: channel_order = "CL_RA"; break;
	case CL_RGB: channel_order = "CL_RGB"; break;
	case CL_RGBA: channel_order = "CL_RGBA"; break;
	case CL_BGRA: channel_order = "CL_BGRA"; break;
	case CL_ARGB: channel_order = "CL_ARGB"; break;
	case CL_INTENSITY: channel_order = "CL_INTENSITY"; break;
	case CL_LUMINANCE: channel_order = "CL_LUMINANCE"; break;
	default: channel_order = "UNKNOWN"; break;
	}

	switch (format.image_channel_data_type) {
	case CL_SNORM_INT8: channel_data_type = "CL_SNORM_INT8"; break;
	case CL_SNORM_INT16: channel_data_type = "CL_SNORM_INT16"; break;
	case CL_UNORM_INT8: channel_data_type = "CL_UNORM_INT8"; break;
	case CL_UNORM_INT16: channel_data_type = "CL_UNORM_INT16"; break;
	case CL_UNORM_SHORT_565: channel_data_type = "CL_UNORM_SHORT_565"; break;
	case CL_UNORM_SHORT_555: channel_data_type = "CL_UNORM_SHORT_555"; break;
	case CL_UNORM_INT_101010: channel_data_type = "CL_UNORM_INT_101010"; break;
	case CL_SIGNED_INT8: channel_data_type = "CL_SIGNED_INT8"; break;
	case CL_SIGNED_INT16: channel_data_type = "CL_SIGNED_INT16"; break;
	case CL_SIGNED_INT32: channel_data_type = "CL_SIGNED_INT32"; break;
	case CL_UNSIGNED_INT8: channel_data_type = "CL_UNSIGNED_INT8"; break;
	case CL_UNSIGNED_INT16: channel_data_type = "CL_UNSIGNED_INT16"; break;
	case CL_UNSIGNED_INT32: channel_data_type = "CL_UNSIGNED_INT32"; break;
	case CL_HALF_FLOAT: channel_data_type = "CL_HALF_FLOAT"; break;
	case CL_FLOAT: channel_data_type = "CL_FLOAT"; break;
	default: channel_data_type = "UNKNOWN"; break;
	}

	return channel_order + Amino::String(" ") + channel_data_type;
}

void Hyuu::OpenCL::Utilitiy::opencl_device_image_formats(ArrayPtr<Amino::String>& formats) {
	std::vector<cl::ImageFormat> _formats;
	CL_CONTEXT.getSupportedImageFormats(CL_MEM_READ_WRITE, CL_MEM_OBJECT_IMAGE2D, &_formats);

	auto _data = Array<Amino::String>();

	for (auto& format : _formats) {
		_data.push_back(formatToString(format));
	}

	formats = Amino::newClassPtr<Array<Amino::String>>(std::move(_data));
}


// Memory::Buffer ======================================================================================================

void Hyuu::OpenCL::Memory::allocate_buffer(MemoryType memory_type, Amino::ulong_t size_bytes, Amino::MutablePtr<Buffer>& buffer) {
	buffer = Amino::newMutablePtr<Buffer>();
	buffer->m_clbuffer = cl::Buffer(CL_CONTEXT, memory_type_to_cl(memory_type), size_bytes);
	buffer->valid = 1;
}

void Hyuu::OpenCL::Memory::allocate_local_buffer(Amino::ulong_t size_bytes, Amino::MutablePtr<LocalBuffer>& buffer) {
	buffer = Amino::newMutablePtr<LocalBuffer>();
	buffer->m_size = size_bytes;
}

void Hyuu::OpenCL::Memory::type_size(const float_t& type, Amino::ulong_t& size_bytes) { size_bytes = sizeof(type); }
void Hyuu::OpenCL::Memory::type_size(const float2& type, Amino::ulong_t& size_bytes) { size_bytes = sizeof(type); }
void Hyuu::OpenCL::Memory::type_size(const float4& type, Amino::ulong_t& size_bytes) { size_bytes = sizeof(type); }
void Hyuu::OpenCL::Memory::type_size(const int_t& type, Amino::ulong_t& size_bytes) { size_bytes = sizeof(type); }
void Hyuu::OpenCL::Memory::type_size(const int2& type, Amino::ulong_t& size_bytes) { size_bytes = sizeof(type); }
void Hyuu::OpenCL::Memory::type_size(const int4& type, Amino::ulong_t& size_bytes) { size_bytes = sizeof(type); }
void Hyuu::OpenCL::Memory::type_size(const uint_t& type, Amino::ulong_t& size_bytes) { size_bytes = sizeof(type); }
void Hyuu::OpenCL::Memory::type_size(const uint2& type, Amino::ulong_t& size_bytes) { size_bytes = sizeof(type); }
void Hyuu::OpenCL::Memory::type_size(const uint4& type, Amino::ulong_t& size_bytes) { size_bytes = sizeof(type); }
void Hyuu::OpenCL::Memory::type_size(const uchar_t& type, Amino::ulong_t& size_bytes) { size_bytes = sizeof(type); }

template <typename T>
Amino::MutablePtr<Hyuu::OpenCL::Memory::Buffer> write_buffer_of_type(const Hyuu::OpenCL::Memory::MemoryType memory_type, const Amino::long_t& size, void* data)
{
	auto buffer = Amino::newMutablePtr<Hyuu::OpenCL::Memory::Buffer>();
	if (!initOpenCL()) {
		return buffer;
	}

	cl_mem_flags flags = memory_type_to_cl(memory_type) | CL_MEM_COPY_HOST_PTR;
	buffer->m_clbuffer = cl::Buffer(CL_CONTEXT, flags, size * sizeof(T), data);

	buffer->m_memory_type = memory_type;
	buffer->valid = 1;
	return buffer;
}


void Hyuu::OpenCL::Memory::write_buffer(MemoryType memory_type, const Array<float_t>& data, Amino::MutablePtr<Buffer>& buffer) { buffer = write_buffer_of_type<float_t>(memory_type, data.size(), (void*)data.data()); }
void Hyuu::OpenCL::Memory::write_buffer(MemoryType memory_type, const Array<float2>& data, Amino::MutablePtr<Buffer>& buffer) { buffer = write_buffer_of_type<float2>(memory_type, data.size(), (void*)data.data()); }
void Hyuu::OpenCL::Memory::write_buffer(MemoryType memory_type, const Array<float4>& data, Amino::MutablePtr<Buffer>& buffer) { buffer = write_buffer_of_type<float4>(memory_type, data.size(), (void*)data.data()); }
void Hyuu::OpenCL::Memory::write_buffer(MemoryType memory_type, const Array<int_t>& data, Amino::MutablePtr<Buffer>& buffer) { buffer = write_buffer_of_type<int_t>(memory_type, data.size(), (void*)data.data()); }
void Hyuu::OpenCL::Memory::write_buffer(MemoryType memory_type, const Array<int2>& data, Amino::MutablePtr<Buffer>& buffer) { buffer = write_buffer_of_type<int2>(memory_type, data.size(), (void*)data.data()); }
void Hyuu::OpenCL::Memory::write_buffer(MemoryType memory_type, const Array<int4>& data, Amino::MutablePtr<Buffer>& buffer) { buffer = write_buffer_of_type<int4>(memory_type, data.size(), (void*)data.data()); }
void Hyuu::OpenCL::Memory::write_buffer(MemoryType memory_type, const Array<uint_t>& data, Amino::MutablePtr<Buffer>& buffer) { buffer = write_buffer_of_type<uint_t>(memory_type, data.size(), (void*)data.data()); }
void Hyuu::OpenCL::Memory::write_buffer(MemoryType memory_type, const Array<uint2>& data, Amino::MutablePtr<Buffer>& buffer) { buffer = write_buffer_of_type<uint2>(memory_type, data.size(), (void*)data.data()); }
void Hyuu::OpenCL::Memory::write_buffer(MemoryType memory_type, const Array<uint4>& data, Amino::MutablePtr<Buffer>& buffer) { buffer = write_buffer_of_type<uint4>(memory_type, data.size(), (void*)data.data()); }
void Hyuu::OpenCL::Memory::write_buffer(MemoryType memory_type, const Array<uchar_t>& data, Amino::MutablePtr<Buffer>& buffer) { buffer = write_buffer_of_type<uchar_t>(memory_type, data.size(), (void*)data.data()); }



template <typename T>
ArrayPtr<T> read_buffer_of_type(const Hyuu::OpenCL::Memory::Buffer& buffer, bool slice, long_t start, long_t length)
{
	if (!buffer.valid) {
		return Amino::newClassPtr<Array<T>>();
	}

	size_t size_bytes = buffer.m_clbuffer.getInfo<CL_MEM_SIZE>();
	long_t count = size_bytes / sizeof(T);
	
	size_t start_bytes, length_bytes;
	if (slice) {
		start = std::min(std::max(start, 0LL), count);
		length = std::min(std::max(length, 0LL), count - start);
		start_bytes = start * sizeof(T);
		length_bytes = length * sizeof(T);
	}
	else {
		length = count;
		start_bytes = 0;
		length_bytes = size_bytes;
	}

	auto _data = Array<T>(length);
	CL_QUEUE.enqueueReadBuffer(buffer.m_clbuffer, CL_TRUE, start_bytes, length_bytes, _data.data());
	CL_QUEUE.finish();
	return Amino::newClassPtr<Array<T>>(std::move(_data));
}

void Hyuu::OpenCL::Memory::read_buffer(const Buffer& buffer, const float_t& type, Amino::bool_t slice, long_t start, long_t length, ArrayPtr<float_t>& data) { data = read_buffer_of_type<float_t>(buffer, slice, start, length); }
void Hyuu::OpenCL::Memory::read_buffer(const Buffer& buffer, const float2& type, Amino::bool_t slice, long_t start, long_t length, ArrayPtr<float2>& data) { data = read_buffer_of_type<float2>(buffer, slice, start, length); }
void Hyuu::OpenCL::Memory::read_buffer(const Buffer& buffer, const float4& type, Amino::bool_t slice, long_t start, long_t length, ArrayPtr<float4>& data) { data = read_buffer_of_type<float4>(buffer, slice, start, length); }
void Hyuu::OpenCL::Memory::read_buffer(const Buffer& buffer, const int_t& type, Amino::bool_t slice, long_t start, long_t length, ArrayPtr<int_t>& data) { data = read_buffer_of_type<int_t>(buffer, slice, start, length); }
void Hyuu::OpenCL::Memory::read_buffer(const Buffer& buffer, const int2& type, Amino::bool_t slice, long_t start, long_t length, ArrayPtr<int2>& data) { data = read_buffer_of_type<int2>(buffer, slice, start, length); }
void Hyuu::OpenCL::Memory::read_buffer(const Buffer& buffer, const int4& type, Amino::bool_t slice, long_t start, long_t length, ArrayPtr<int4>& data) { data = read_buffer_of_type<int4>(buffer, slice, start, length); }
void Hyuu::OpenCL::Memory::read_buffer(const Buffer& buffer, const uint_t& type, Amino::bool_t slice, long_t start, long_t length, ArrayPtr<uint_t>& data) { data = read_buffer_of_type<uint_t>(buffer, slice, start, length); }
void Hyuu::OpenCL::Memory::read_buffer(const Buffer& buffer, const uint2& type, Amino::bool_t slice, long_t start, long_t length, ArrayPtr<uint2>& data) { data = read_buffer_of_type<uint2>(buffer, slice, start, length); }
void Hyuu::OpenCL::Memory::read_buffer(const Buffer& buffer, const uint4& type, Amino::bool_t slice, long_t start, long_t length, ArrayPtr<uint4>& data) { data = read_buffer_of_type<uint4>(buffer, slice, start, length); }
void Hyuu::OpenCL::Memory::read_buffer(const Buffer& buffer, const uchar_t& type, Amino::bool_t slice, long_t start, long_t length, ArrayPtr<uchar_t>& data) { data = read_buffer_of_type<uchar_t>(buffer, slice, start, length); }


void Hyuu::OpenCL::Memory::buffer_size_bytes(const Buffer& buffer, Amino::long_t& size_bytes) {
	if (!buffer.valid) {
		size_bytes = 0;
		return;
	}
	size_bytes = buffer.m_clbuffer.getInfo<CL_MEM_SIZE>();
}

void Hyuu::OpenCL::Memory::buffer_size_bytes(const LocalBuffer& buffer, Amino::long_t& size_bytes) {
	size_bytes = buffer.m_size;
}

void Hyuu::OpenCL::Memory::buffer_size_bytes(const Image::Image1DBuffer& buffer, Amino::long_t& size_bytes) {
	if (!buffer.valid) {
		size_bytes = 0;
		return;
	}
	size_bytes = buffer.m_climage.getInfo<CL_MEM_SIZE>();
}

void Hyuu::OpenCL::Memory::buffer_size_bytes(const Image::Image2DBuffer& buffer, Amino::long_t& size_bytes) {
	if (!buffer.valid) {
		size_bytes = 0;
		return;
	}
	size_bytes = buffer.m_climage.getInfo<CL_MEM_SIZE>();
}

void Hyuu::OpenCL::Memory::buffer_size_bytes(const Image::Image3DBuffer& buffer, Amino::long_t& size_bytes) {
	if (!buffer.valid) {
		size_bytes = 0;
		return;
	}
	size_bytes = buffer.m_climage.getInfo<CL_MEM_SIZE>();
}


void Hyuu::OpenCL::Memory::copy_buffer(Buffer& target AMINO_ANNOTATE("Amino::InOut outName=buffer_out"), const Buffer& source) {
	if (!source.valid || !target.valid)
		return;

	CL_QUEUE.enqueueCopyBuffer(source.m_clbuffer, target.m_clbuffer, 0, 0, target.m_clbuffer.getInfo<CL_MEM_SIZE>());
}

void Hyuu::OpenCL::Memory::copy_buffer(Image::Image1DBuffer& target AMINO_ANNOTATE("Amino::InOut outName=buffer_out"), const Image::Image1DBuffer& source) {
	if (!source.valid || !target.valid)
		return;

	auto width = target.m_climage.getImageInfo<CL_IMAGE_WIDTH>();
	CL_QUEUE.enqueueCopyImage(source.m_climage, target.m_climage, { 0, 0, 0 }, { 0, 0, 0 }, { width, 1, 1 });
}

void Hyuu::OpenCL::Memory::copy_buffer(Image::Image2DBuffer& target AMINO_ANNOTATE("Amino::InOut outName=buffer_out"), const Image::Image2DBuffer& source) {
	if (!source.valid || !target.valid)
		return;

	auto width = target.m_climage.getImageInfo<CL_IMAGE_WIDTH>();
	auto height = target.m_climage.getImageInfo<CL_IMAGE_HEIGHT>();
	CL_QUEUE.enqueueCopyImage(source.m_climage, target.m_climage, { 0, 0, 0 }, { 0, 0, 0 }, { width, height, 1 });
}

void Hyuu::OpenCL::Memory::copy_buffer(Image::Image3DBuffer& target AMINO_ANNOTATE("Amino::InOut outName=buffer_out"), const Image::Image3DBuffer& source) {
	if (!source.valid || !target.valid)
		return;

	auto width = target.m_climage.getImageInfo<CL_IMAGE_WIDTH>();
	auto height = target.m_climage.getImageInfo<CL_IMAGE_HEIGHT>();
	auto depth = target.m_climage.getImageInfo<CL_IMAGE_DEPTH>();
	CL_QUEUE.enqueueCopyImage(source.m_climage, target.m_climage, { 0, 0, 0 }, { 0, 0, 0 }, { width, height, depth });
}


// Memory::Image =======================================================================================================

// Allocate Image Buffer ----------------------------------------------------------------------------------------------

int Hyuu::OpenCL::Memory::Image::getCopyCount(const Image2DBuffer& buffer) {
	return buffer.valid;
}

Amino::MutablePtr<Hyuu::OpenCL::Memory::Image::Image1DBuffer> allocate_image1d(Hyuu::OpenCL::Memory::MemoryType memory_type, const uint_t& dimensions, int format) {
	auto buffer = Amino::newMutablePtr<Hyuu::OpenCL::Memory::Image::Image1DBuffer>();
	if (!initOpenCL()) {
		return buffer;
	}
	buffer->m_climage = cl::Image1D(CL_CONTEXT, memory_type_to_cl(memory_type), cl::ImageFormat(format, CL_FLOAT), dimensions);
	buffer->m_memory_type = memory_type;
	buffer->valid = 1;
	return buffer;

}

Amino::MutablePtr<Hyuu::OpenCL::Memory::Image::Image2DBuffer> allocate_image2d(Hyuu::OpenCL::Memory::MemoryType memory_type, const uint2& dimensions, int format) {
	auto buffer = Amino::newMutablePtr<Hyuu::OpenCL::Memory::Image::Image2DBuffer>();
	if (!initOpenCL()) {
		return buffer;
	}
	buffer->m_climage = cl::Image2D(CL_CONTEXT, memory_type_to_cl(memory_type), cl::ImageFormat(format, CL_FLOAT), dimensions.x, dimensions.y);
	buffer->m_memory_type = memory_type;
	buffer->valid = 1;
	return buffer;

}

Amino::MutablePtr<Hyuu::OpenCL::Memory::Image::Image3DBuffer> allocate_image3d(Hyuu::OpenCL::Memory::MemoryType memory_type, const uint3& dimensions, int format) {
	auto buffer = Amino::newMutablePtr<Hyuu::OpenCL::Memory::Image::Image3DBuffer>();
	if (!initOpenCL()) {
		return buffer;
	}
	buffer->m_climage = cl::Image3D(CL_CONTEXT, memory_type_to_cl(memory_type), cl::ImageFormat(format, CL_FLOAT), dimensions.x, dimensions.y, dimensions.z);
	buffer->m_memory_type = memory_type;
	buffer->valid = 1;
	return buffer;
}

void Hyuu::OpenCL::Memory::Image::allocate_image_buffer(MemoryType memory_type, const uint_t& dimensions, const float_t& type, Amino::MutablePtr<Image1DBuffer>& buffer) { buffer = allocate_image1d(memory_type, dimensions, CL_R); }
void Hyuu::OpenCL::Memory::Image::allocate_image_buffer(MemoryType memory_type, const uint_t& dimensions, const float2& type, Amino::MutablePtr<Image1DBuffer>& buffer) { buffer = allocate_image1d(memory_type, dimensions, CL_RG); }
void Hyuu::OpenCL::Memory::Image::allocate_image_buffer(MemoryType memory_type, const uint_t& dimensions, const float3& type, Amino::MutablePtr<Image1DBuffer>& buffer) { buffer = allocate_image1d(memory_type, dimensions, CL_RGB); }
void Hyuu::OpenCL::Memory::Image::allocate_image_buffer(MemoryType memory_type, const uint_t& dimensions, const float4& type, Amino::MutablePtr<Image1DBuffer>& buffer) { buffer = allocate_image1d(memory_type, dimensions, CL_RGBA); }

void Hyuu::OpenCL::Memory::Image::allocate_image_buffer(MemoryType memory_type, const uint2& dimensions, const float_t& type, Amino::MutablePtr<Image2DBuffer>& buffer) { buffer = allocate_image2d(memory_type, dimensions, CL_R); }
void Hyuu::OpenCL::Memory::Image::allocate_image_buffer(MemoryType memory_type, const uint2& dimensions, const float2& type, Amino::MutablePtr<Image2DBuffer>& buffer) { buffer = allocate_image2d(memory_type, dimensions, CL_RG); }
void Hyuu::OpenCL::Memory::Image::allocate_image_buffer(MemoryType memory_type, const uint2& dimensions, const float3& type, Amino::MutablePtr<Image2DBuffer>& buffer) { buffer = allocate_image2d(memory_type, dimensions, CL_RGB); }
void Hyuu::OpenCL::Memory::Image::allocate_image_buffer(MemoryType memory_type, const uint2& dimensions, const float4& type, Amino::MutablePtr<Image2DBuffer>& buffer) { buffer = allocate_image2d(memory_type, dimensions, CL_RGBA); }

void Hyuu::OpenCL::Memory::Image::allocate_image_buffer(MemoryType memory_type, const uint3& dimensions, const float_t& type, Amino::MutablePtr<Image3DBuffer>& buffer) { buffer = allocate_image3d(memory_type, dimensions, CL_R); }
void Hyuu::OpenCL::Memory::Image::allocate_image_buffer(MemoryType memory_type, const uint3& dimensions, const float2& type, Amino::MutablePtr<Image3DBuffer>& buffer) { buffer = allocate_image3d(memory_type, dimensions, CL_RG); }
void Hyuu::OpenCL::Memory::Image::allocate_image_buffer(MemoryType memory_type, const uint3& dimensions, const float3& type, Amino::MutablePtr<Image3DBuffer>& buffer) { buffer = allocate_image3d(memory_type, dimensions, CL_RGB); }
void Hyuu::OpenCL::Memory::Image::allocate_image_buffer(MemoryType memory_type, const uint3& dimensions, const float4& type, Amino::MutablePtr<Image3DBuffer>& buffer) { buffer = allocate_image3d(memory_type, dimensions, CL_RGBA); }
// ---------------------------------------------------------------------------------------------------------------------

// Write Image Buffer --------------------------------------------------------------------------------------------------
Amino::MutablePtr<Hyuu::OpenCL::Memory::Image::Image1DBuffer> write_image1d(Hyuu::OpenCL::Memory::MemoryType memory_type, const uint_t& dimensions, int format, void* data) {
	auto buffer = Amino::newMutablePtr<Hyuu::OpenCL::Memory::Image::Image1DBuffer>();
	if (!initOpenCL()) {
		return buffer;
	}

	cl_mem_flags flags = memory_type_to_cl(memory_type) | CL_MEM_COPY_HOST_PTR;
	buffer->m_climage = cl::Image1D(CL_CONTEXT, flags, cl::ImageFormat(format, CL_FLOAT), dimensions, data);
	buffer->m_memory_type = memory_type;
	buffer->valid = 1;
	return buffer;
}

Amino::MutablePtr<Hyuu::OpenCL::Memory::Image::Image2DBuffer> write_image2d(Hyuu::OpenCL::Memory::MemoryType memory_type, const uint2& dimensions, int format, void* data) {
	auto buffer = Amino::newMutablePtr<Hyuu::OpenCL::Memory::Image::Image2DBuffer>();
	if (!initOpenCL()) {
		return buffer;
	}

	cl_mem_flags flags = memory_type_to_cl(memory_type) | CL_MEM_COPY_HOST_PTR;
	buffer->m_climage = cl::Image2D(CL_CONTEXT, flags, cl::ImageFormat(format, CL_FLOAT), dimensions.x, dimensions.y, 0, data);
	buffer->m_memory_type = memory_type;
	buffer->valid = 1;
	return buffer;
}

Amino::MutablePtr<Hyuu::OpenCL::Memory::Image::Image3DBuffer> write_image3d(Hyuu::OpenCL::Memory::MemoryType memory_type, const uint3& dimensions, int format, void* data) {
	auto buffer = Amino::newMutablePtr<Hyuu::OpenCL::Memory::Image::Image3DBuffer>();
	if (!initOpenCL()) {
		return buffer;
	}

	cl_mem_flags flags = memory_type_to_cl(memory_type) | CL_MEM_COPY_HOST_PTR;
	buffer->m_climage = cl::Image3D(CL_CONTEXT, flags, cl::ImageFormat(format, CL_FLOAT), dimensions.x, dimensions.y, dimensions.z, 0, 0, data);
	buffer->m_memory_type = memory_type;
	buffer->valid = 1;
	return buffer;
}

void Hyuu::OpenCL::Memory::Image::write_image_buffer(MemoryType memory_type, const Array<float_t>& data, const uint_t& dimensions, Amino::MutablePtr<Image1DBuffer>& buffer) { buffer = write_image1d(memory_type, dimensions, CL_R, (void*)data.data()); }
void Hyuu::OpenCL::Memory::Image::write_image_buffer(MemoryType memory_type, const Array<float2>& data, const uint_t& dimensions, Amino::MutablePtr<Image1DBuffer>& buffer) { buffer = write_image1d(memory_type, dimensions, CL_RG, (void*)data.data()); }
void Hyuu::OpenCL::Memory::Image::write_image_buffer(MemoryType memory_type, const Array<float3>& data, const uint_t& dimensions, Amino::MutablePtr<Image1DBuffer>& buffer) { buffer = write_image1d(memory_type, dimensions, CL_RGB, (void*)data.data()); }
void Hyuu::OpenCL::Memory::Image::write_image_buffer(MemoryType memory_type, const Array<float4>& data, const uint_t& dimensions, Amino::MutablePtr<Image1DBuffer>& buffer) { buffer = write_image1d(memory_type, dimensions, CL_RGBA, (void*)data.data()); }

void Hyuu::OpenCL::Memory::Image::write_image_buffer(MemoryType memory_type, const Array<float_t>& data, const uint2& dimensions, Amino::MutablePtr<Image2DBuffer>& buffer) { buffer = write_image2d(memory_type, dimensions, CL_R, (void*)data.data()); }
void Hyuu::OpenCL::Memory::Image::write_image_buffer(MemoryType memory_type, const Array<float2>& data, const uint2& dimensions, Amino::MutablePtr<Image2DBuffer>& buffer) { buffer = write_image2d(memory_type, dimensions, CL_RG, (void*)data.data()); }
void Hyuu::OpenCL::Memory::Image::write_image_buffer(MemoryType memory_type, const Array<float3>& data, const uint2& dimensions, Amino::MutablePtr<Image2DBuffer>& buffer) { buffer = write_image2d(memory_type, dimensions, CL_RGB, (void*)data.data()); }
void Hyuu::OpenCL::Memory::Image::write_image_buffer(MemoryType memory_type, const Array<float4>& data, const uint2& dimensions, Amino::MutablePtr<Image2DBuffer>& buffer) { buffer = write_image2d(memory_type, dimensions, CL_RGBA, (void*)data.data()); }

void Hyuu::OpenCL::Memory::Image::write_image_buffer(MemoryType memory_type, const Array<float_t>& data, const uint3& dimensions, Amino::MutablePtr<Image3DBuffer>& buffer) { buffer = write_image3d(memory_type, dimensions, CL_R, (void*)data.data()); }
void Hyuu::OpenCL::Memory::Image::write_image_buffer(MemoryType memory_type, const Array<float2>& data, const uint3& dimensions, Amino::MutablePtr<Image3DBuffer>& buffer) { buffer = write_image3d(memory_type, dimensions, CL_RG, (void*)data.data()); }
void Hyuu::OpenCL::Memory::Image::write_image_buffer(MemoryType memory_type, const Array<float3>& data, const uint3& dimensions, Amino::MutablePtr<Image3DBuffer>& buffer) { buffer = write_image3d(memory_type, dimensions, CL_RGB, (void*)data.data()); }
void Hyuu::OpenCL::Memory::Image::write_image_buffer(MemoryType memory_type, const Array<float4>& data, const uint3& dimensions, Amino::MutablePtr<Image3DBuffer>& buffer) { buffer = write_image3d(memory_type, dimensions, CL_RGBA, (void*)data.data()); }

// ---------------------------------------------------------------------------------------------------------------------

// Read Image Buffer ---------------------------------------------------------------------------------------------------
template <typename T>
ArrayPtr<T> read_image1d(const Hyuu::OpenCL::Memory::Image::Image1DBuffer& buffer, uint_t& dimensions) {
	if (!buffer.valid) {
		return Amino::newClassPtr<Array<T>>();
	}

	dimensions = buffer.m_climage.getImageInfo<CL_IMAGE_WIDTH>();
	auto _data = Array<T>(dimensions);

	CL_QUEUE.enqueueReadImage(buffer.m_climage, CL_TRUE, { 0, 0, 0 }, { dimensions, 1, 1 }, 0, 0, _data.data());
	return Amino::newClassPtr<Array<T>>(std::move(_data));
}

template <typename T>
ArrayPtr<T> read_image2d(const Hyuu::OpenCL::Memory::Image::Image2DBuffer& buffer, uint2& dimensions) {
	if (!buffer.valid) {
		return Amino::newClassPtr<Array<T>>();
	}

	dimensions.x = buffer.m_climage.getImageInfo<CL_IMAGE_WIDTH>();
	dimensions.y = buffer.m_climage.getImageInfo<CL_IMAGE_HEIGHT>();
	auto _data = Array<T>(dimensions.x * dimensions.y);

	CL_QUEUE.enqueueReadImage(buffer.m_climage, CL_TRUE, { 0, 0, 0 }, { dimensions.x, dimensions.y, 1 }, 0, 0, _data.data());
	return Amino::newClassPtr<Array<T>>(std::move(_data));
}

template <typename T>
ArrayPtr<T> read_image3d(const Hyuu::OpenCL::Memory::Image::Image3DBuffer& buffer, uint3& dimensions) {
	if (!buffer.valid) {
		return Amino::newClassPtr<Array<T>>();
	}

	dimensions.x = buffer.m_climage.getImageInfo<CL_IMAGE_WIDTH>();
	dimensions.y = buffer.m_climage.getImageInfo<CL_IMAGE_HEIGHT>();
	dimensions.z = buffer.m_climage.getImageInfo<CL_IMAGE_DEPTH>();
	auto _data = Array<T>(dimensions.x * dimensions.y * dimensions.z);

	CL_QUEUE.enqueueReadImage(buffer.m_climage, CL_TRUE, { 0, 0, 0 }, { dimensions.x, dimensions.y, dimensions.z }, 0, 0, _data.data());
	return Amino::newClassPtr<Array<T>>(std::move(_data));
}

void Hyuu::OpenCL::Memory::Image::read_image_buffer(const Image1DBuffer& buffer, const float_t& type, ArrayPtr<float_t>& data, uint_t& dimensions) { data = read_image1d<float_t>(buffer, dimensions); }
void Hyuu::OpenCL::Memory::Image::read_image_buffer(const Image1DBuffer& buffer, const float2& type, ArrayPtr<float2>& data, uint_t& dimensions) { data = read_image1d<float2>(buffer, dimensions); }
void Hyuu::OpenCL::Memory::Image::read_image_buffer(const Image1DBuffer& buffer, const float3& type, ArrayPtr<float3>& data, uint_t& dimensions) { data = read_image1d<float3>(buffer, dimensions); }
void Hyuu::OpenCL::Memory::Image::read_image_buffer(const Image1DBuffer& buffer, const float4& type, ArrayPtr<float4>& data, uint_t& dimensions) { data = read_image1d<float4>(buffer, dimensions); }

void Hyuu::OpenCL::Memory::Image::read_image_buffer(const Image2DBuffer& buffer, const float_t& type, ArrayPtr<float_t>& data, uint2& dimensions) { data = read_image2d<float_t>(buffer, dimensions); }
void Hyuu::OpenCL::Memory::Image::read_image_buffer(const Image2DBuffer& buffer, const float2& type, ArrayPtr<float2>& data, uint2& dimensions) { data = read_image2d<float2>(buffer, dimensions); }
void Hyuu::OpenCL::Memory::Image::read_image_buffer(const Image2DBuffer& buffer, const float3& type, ArrayPtr<float3>& data, uint2& dimensions) { data = read_image2d<float3>(buffer, dimensions); }
void Hyuu::OpenCL::Memory::Image::read_image_buffer(const Image2DBuffer& buffer, const float4& type, ArrayPtr<float4>& data, uint2& dimensions) { data = read_image2d<float4>(buffer, dimensions); }

void Hyuu::OpenCL::Memory::Image::read_image_buffer(const Image3DBuffer& buffer, const float_t& type, ArrayPtr<float_t>& data, uint3& dimensions) { data = read_image3d<float_t>(buffer, dimensions); }
void Hyuu::OpenCL::Memory::Image::read_image_buffer(const Image3DBuffer& buffer, const float2& type, ArrayPtr<float2>& data, uint3& dimensions) { data = read_image3d<float2>(buffer, dimensions); }
void Hyuu::OpenCL::Memory::Image::read_image_buffer(const Image3DBuffer& buffer, const float3& type, ArrayPtr<float3>& data, uint3& dimensions) { data = read_image3d<float3>(buffer, dimensions); }
void Hyuu::OpenCL::Memory::Image::read_image_buffer(const Image3DBuffer& buffer, const float4& type, ArrayPtr<float4>& data, uint3& dimensions) { data = read_image3d<float4>(buffer, dimensions); }

// ---------------------------------------------------------------------------------------------------------------------

// Get Channel Format --------------------------------------------------------------------------------------------------
void convertFormat(const cl_image_format cl_image_format, Hyuu::OpenCL::Memory::Image::ChannelOrder& channel_order, Hyuu::OpenCL::Memory::Image::ChannelType& channel_type)
{
	switch (cl_image_format.image_channel_order) {
	case CL_R:
		channel_order = Hyuu::OpenCL::Memory::Image::ChannelOrder::R;
		break;
	case CL_RG:
		channel_order = Hyuu::OpenCL::Memory::Image::ChannelOrder::RG;
		break;
	case CL_RGB:
		channel_order = Hyuu::OpenCL::Memory::Image::ChannelOrder::RGB;
		break;
	case CL_RGBA:
		channel_order = Hyuu::OpenCL::Memory::Image::ChannelOrder::RGBA;
		break;
	default:
		channel_order = Hyuu::OpenCL::Memory::Image::ChannelOrder::R;
		break;
	}

	channel_type = Hyuu::OpenCL::Memory::Image::ChannelType::FLOAT;  // only supported type at the moment
}

void Hyuu::OpenCL::Memory::Image::get_image_format(const Image1DBuffer& buffer, ChannelOrder& channel_order, ChannelType& channel_type) {
	if (!buffer.valid) {
		channel_order = ChannelOrder::R;
		channel_type = ChannelType::FLOAT;
		return;
	}

	auto cl_image_format = buffer.m_climage.getImageInfo<CL_IMAGE_FORMAT>();
	convertFormat(cl_image_format, channel_order, channel_type);
}

void Hyuu::OpenCL::Memory::Image::get_image_format(const Image2DBuffer& buffer, ChannelOrder& channel_order, ChannelType& channel_type) {
	if (!buffer.valid) {
		channel_order = ChannelOrder::R;
		channel_type = ChannelType::FLOAT;
		return;
	}

	auto cl_image_format = buffer.m_climage.getImageInfo<CL_IMAGE_FORMAT>();
	convertFormat(cl_image_format, channel_order, channel_type);
}

void Hyuu::OpenCL::Memory::Image::get_image_format(const Image3DBuffer& buffer, ChannelOrder& channel_order, ChannelType& channel_type) {
	if (!buffer.valid) {
		channel_order = ChannelOrder::R;
		channel_type = ChannelType::FLOAT;
		return;
	}

	auto cl_image_format = buffer.m_climage.getImageInfo<CL_IMAGE_FORMAT>();
	convertFormat(cl_image_format, channel_order, channel_type);
}

// Program/Kernel ======================================================================================================

void Hyuu::OpenCL::Execute::build_program(const Amino::String& source, Amino::MutablePtr<Bifrost::Object>& kernels, Amino::String& log, bool& success) {
	kernels = Bifrost::createObject();
	success = false;
	CL_INIT_CHECK();

	// Build and add log to object
	cl_int err;
	cl::Program clprogram = cl::Program(CL_CONTEXT, source.c_str(), /*build*/ true, &err);
	log = clprogram.getBuildInfo<CL_PROGRAM_BUILD_LOG>(CL_DEVICE);

	// Return if build failed
	if (err != CL_SUCCESS)
		return;

	// Create sub object of kernels
	std::vector<cl::Kernel> _kernels;
	clprogram.createKernels(&_kernels);
	for (auto& kernel : _kernels) {
		auto _kernel = Amino::newMutablePtr<Kernel>();
		_kernel->m_clkernel = kernel;
		_kernel->valid = 1;
		_kernel->m_args.resize(kernel.getInfo<CL_KERNEL_NUM_ARGS>());
		std::string name;
		kernel.getInfo(CL_KERNEL_FUNCTION_NAME, &name);
		kernels->setProperty(name, _kernel.toImmutable());
	}

	success = true;
}


// Set Arg =============================================================================================================

bool kernelArgCheck(const Hyuu::OpenCL::Execute::Kernel& kernel, Amino::long_t arg_id) {
	if (!kernel.valid) {
		return false;
	}

	// arg id out of range
	if (arg_id >= kernel.m_args.size()) {
		return false;
	}

	return true;
}

// Set Arg Buffer -----------------------------------------------------------------------------------------------------
void Hyuu::OpenCL::Execute::set_kernel_arg(Kernel& kernel, Amino::long_t arg_id, const Memory::Buffer& value, bool& success) {
	success = kernelArgCheck(kernel, arg_id);
	if (!success)
		return;

	auto result = kernel.m_clkernel.setArg(arg_id, value.m_clbuffer);
	kernel.m_args[arg_id] = Amino::newClassPtr<Memory::Buffer>(value);
	success = result == CL_SUCCESS;
}


void Hyuu::OpenCL::Execute::set_kernel_arg(Kernel& kernel, Amino::long_t arg_id, const Memory::LocalBuffer& value, bool& success) {
	success = kernelArgCheck(kernel, arg_id);
	if (!success)
		return;

	auto result = kernel.m_clkernel.setArg(arg_id, value.m_size, nullptr);
	kernel.m_args[arg_id] = Amino::newClassPtr<Memory::LocalBuffer>(value);
	success = result == CL_SUCCESS;
}
// ---------------------------------------------------------------------------------------------------------------------

// Set Arg Image ------------------------------------------------------------------------------------------------------
void Hyuu::OpenCL::Execute::set_kernel_arg(Kernel& kernel, Amino::long_t arg_id, const Memory::Image::ImageSampler& value, bool& success) {
	success = kernelArgCheck(kernel, arg_id);
	if (!success)
		return;

	auto bounds_mode = Memory::Image::getSampleBounds(value.bounds);
	auto interpolation_mode = Memory::Image::getSampleInterpolation(value.interpolation);
	cl::Sampler sampler(CL_CONTEXT, value.normalized_coords, bounds_mode, interpolation_mode);
	auto result = kernel.m_clkernel.setArg(arg_id, sampler);
	kernel.m_args[arg_id] = Amino::newClassPtr<Memory::Image::ClSamplerWrapper>(sampler);
	success = result == CL_SUCCESS;
}

template <typename T>
bool set_kernel_arg_image(Hyuu::OpenCL::Execute::Kernel& kernel, Amino::long_t arg_id, const T& value) {
	if (!kernelArgCheck(kernel, arg_id))
		return false;

	auto result = kernel.m_clkernel.setArg(arg_id, value.m_climage);
	kernel.m_args[arg_id] = Amino::newClassPtr<T>(value);
	return result == CL_SUCCESS;
}

void Hyuu::OpenCL::Execute::set_kernel_arg(Kernel& kernel, Amino::long_t arg_id, const Memory::Image::Image1DBuffer& value, bool& success) {
	success = set_kernel_arg_image(kernel, arg_id, value);
}

void Hyuu::OpenCL::Execute::set_kernel_arg(Kernel& kernel, Amino::long_t arg_id, const Memory::Image::Image2DBuffer& value, bool& success) {
	success = set_kernel_arg_image(kernel, arg_id, value);
}

void Hyuu::OpenCL::Execute::set_kernel_arg(Kernel& kernel, Amino::long_t arg_id, const Memory::Image::Image3DBuffer& value, bool& success) {
	success = set_kernel_arg_image(kernel, arg_id, value);
}
// ---------------------------------------------------------------------------------------------------------------------

// Set Arg Scalar -----------------------------------------------------------------------------------------------------
template <typename T>
bool set_kernel_arg_scalar(Hyuu::OpenCL::Execute::Kernel& kernel, Amino::long_t arg_id, const T value) {
	if (!kernelArgCheck(kernel, arg_id))
		return false;

	auto result = kernel.m_clkernel.setArg(arg_id, value);
	kernel.m_args[arg_id] = value;
	return result == CL_SUCCESS;
}

void Hyuu::OpenCL::Execute::set_kernel_arg(Kernel& kernel, Amino::long_t arg_id, const float_t& value, bool& success) { success = set_kernel_arg_scalar(kernel, arg_id, value); }
void Hyuu::OpenCL::Execute::set_kernel_arg(Kernel& kernel, Amino::long_t arg_id, const float2& value, bool& success) { success = set_kernel_arg_scalar(kernel, arg_id, value); }
void Hyuu::OpenCL::Execute::set_kernel_arg(Kernel& kernel, Amino::long_t arg_id, const float4& value, bool& success) { success = set_kernel_arg_scalar(kernel, arg_id, value); }
void Hyuu::OpenCL::Execute::set_kernel_arg(Kernel& kernel, Amino::long_t arg_id, const int_t& value, bool& success) { success = set_kernel_arg_scalar(kernel, arg_id, value); }
void Hyuu::OpenCL::Execute::set_kernel_arg(Kernel& kernel, Amino::long_t arg_id, const int2& value, bool& success) { success = set_kernel_arg_scalar(kernel, arg_id, value); }
void Hyuu::OpenCL::Execute::set_kernel_arg(Kernel& kernel, Amino::long_t arg_id, const int4& value, bool& success) { success = set_kernel_arg_scalar(kernel, arg_id, value); }
void Hyuu::OpenCL::Execute::set_kernel_arg(Kernel& kernel, Amino::long_t arg_id, const uint_t& value, bool& success) { success = set_kernel_arg_scalar(kernel, arg_id, value); }
void Hyuu::OpenCL::Execute::set_kernel_arg(Kernel& kernel, Amino::long_t arg_id, const uint2& value, bool& success) { success = set_kernel_arg_scalar(kernel, arg_id, value); }
void Hyuu::OpenCL::Execute::set_kernel_arg(Kernel& kernel, Amino::long_t arg_id, const uint4& value, bool& success) { success = set_kernel_arg_scalar(kernel, arg_id, value); }
void Hyuu::OpenCL::Execute::set_kernel_arg(Kernel& kernel, Amino::long_t arg_id, const uchar_t& value, bool& success) { success = set_kernel_arg_scalar(kernel, arg_id, value); }
// ---------------------------------------------------------------------------------------------------------------------

void Hyuu::OpenCL::Execute::get_kernel_args(const Kernel& kernel, ArrayPtr<Amino::Any>& args) {
	if (!kernel.valid) {
		args = Amino::newClassPtr<Array<Amino::Any>>();
		return;
	}

	args = Amino::newClassPtr<Array<Amino::Any>>(kernel.m_args);
}

// Execute Kernel ======================================================================================================

cl::NDRange arrayToRange(const Array<Amino::ulong_t>& _array) {
	auto array_size = _array.size();
	switch (array_size) {
	case 0:
		return cl::NullRange;
	case 1:
		return cl::NDRange(_array[0]);
	case 2:
		return cl::NDRange(_array[0], _array[1]);
	case 3:
		return cl::NDRange(_array[0], _array[1], _array[2]);
	default:
		return cl::NullRange;
	}
}

void Hyuu::OpenCL::Execute::execute_kernel(
	Kernel& kernel AMINO_ANNOTATE("Amino::InOut outName=kernel_out"),
	const Array<Amino::ulong_t>& global_offset,
	const Array<Amino::ulong_t>& global_size,
	const Array<Amino::ulong_t>& local_size,
	bool& success,
	Amino::String& error
) {
	if (!initOpenCL()) {
		success = false;
		error = "Failed to initialize OpenCL";
		return;
	}

	if (!kernel.valid) {
		success = false;
		error = "Invalid kernel";
		return;
	}

	auto result = CL_QUEUE.enqueueNDRangeKernel(kernel.m_clkernel, arrayToRange(global_offset), arrayToRange(global_size), arrayToRange(local_size));
	CL_QUEUE.finish();
	success = result == CL_SUCCESS;
	if (!success) {
		error = "Failed to execute kernel: " + std::to_string(result);
	}
	else {
		error = "";
	}
}


// ---------------------------------------------------------------------------------------------------------------------