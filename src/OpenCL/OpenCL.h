#include "../Hyuu.h"

#include <CL/opencl.hpp>



// Current supported types for gpu io
// float float2 float4 int int2 int4 uint uint2 uint4 uchar(for bool)

cl::Context& getContext();
cl::CommandQueue& getQueue();

namespace Hyuu {
	namespace OpenCL {
		namespace Utilitiy {
			//HYUU_DECL bool opencl_restart() AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");  // doesn't work it seems, needs more investigation
			HYUU_DECL void opencl_device_info(Amino::MutablePtr<Bifrost::Object>& device_info) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");
		}  // namespace Utilitiy

		namespace Memory {

			enum class AMINO_ANNOTATE("Amino::Enum") MemoryType : int
			{
				READ_WRITE = 0,
					READ = 1,
					WRITE = 2
			};

			cl_mem_flags memory_type_to_cl(Hyuu::OpenCL::Memory::MemoryType memory_type)
			{
				switch (memory_type) {
				case Hyuu::OpenCL::Memory::MemoryType::READ_WRITE:
					return CL_MEM_READ_WRITE;
				case Hyuu::OpenCL::Memory::MemoryType::READ:
					return CL_MEM_READ_ONLY;
				case Hyuu::OpenCL::Memory::MemoryType::WRITE:
					return CL_MEM_WRITE_ONLY;
				default:
					return CL_MEM_READ_WRITE;
				}
			}


			class AMINO_ANNOTATE("Amino::Class") HYUU_DECL Buffer {
			public:
				cl::Buffer m_clbuffer;
				MemoryType m_memory_type;
				int valid = 0;

				Buffer() {
					m_memory_type = MemoryType::READ_WRITE;
				}

				Buffer(const Buffer & input) {
					m_clbuffer = input.m_clbuffer;
					m_memory_type = input.m_memory_type;
					valid = input.valid;
				}

				~Buffer() = default;
			};

			// wrapper for of ulong for indicating size for local memory allocation
			class AMINO_ANNOTATE("Amino::Class") HYUU_DECL LocalBuffer {
			public:
				size_t m_size;

				LocalBuffer() = default;

				LocalBuffer(const LocalBuffer & input) {
					m_size = input.m_size;
				}

				~LocalBuffer() = default;
			};

			HYUU_DECL
				void allocate_buffer(MemoryType memory_type, Amino::ulong_t size_bytes, Amino::MutablePtr<Buffer>& buffer) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");

			HYUU_DECL
				void allocate_local_buffer(Amino::ulong_t size_bytes, Amino::MutablePtr<LocalBuffer>& buffer) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");

			HYUU_DECL void type_size(const float_t& type, Amino::ulong_t& size_bytes) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");
			HYUU_DECL void type_size(const float2& type, Amino::ulong_t& size_bytes) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");
			HYUU_DECL void type_size(const float4& type, Amino::ulong_t& size_bytes) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");
			HYUU_DECL void type_size(const Amino::int_t& type, Amino::ulong_t& size_bytes) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");
			HYUU_DECL void type_size(const int2& type, Amino::ulong_t& size_bytes) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");
			HYUU_DECL void type_size(const int4& type, Amino::ulong_t& size_bytes) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");
			HYUU_DECL void type_size(const Amino::uint_t& type, Amino::ulong_t& size_bytes) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");
			HYUU_DECL void type_size(const uint2& type, Amino::ulong_t& size_bytes) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");
			HYUU_DECL void type_size(const uint4& type, Amino::ulong_t& size_bytes) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");
			HYUU_DECL void type_size(const Amino::uchar_t& type, Amino::ulong_t& size_bytes) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");

			HYUU_DECL void write_buffer(MemoryType memory_type, const Array<float_t>& data, Amino::MutablePtr<Buffer>& buffer) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");
			HYUU_DECL void write_buffer(MemoryType memory_type, const Array<float2>& data, Amino::MutablePtr<Buffer>& buffer) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");
			HYUU_DECL void write_buffer(MemoryType memory_type, const Array<float4>& data, Amino::MutablePtr<Buffer>& buffer) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");
			HYUU_DECL void write_buffer(MemoryType memory_type, const Array<int_t>& data, Amino::MutablePtr<Buffer>& buffer) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");
			HYUU_DECL void write_buffer(MemoryType memory_type, const Array<int2>& data, Amino::MutablePtr<Buffer>& buffer) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");
			HYUU_DECL void write_buffer(MemoryType memory_type, const Array<int4>& data, Amino::MutablePtr<Buffer>& buffer) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");
			HYUU_DECL void write_buffer(MemoryType memory_type, const Array<uint_t>& data, Amino::MutablePtr<Buffer>& buffer) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");
			HYUU_DECL void write_buffer(MemoryType memory_type, const Array<uint2>& data, Amino::MutablePtr<Buffer>& buffer) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");
			HYUU_DECL void write_buffer(MemoryType memory_type, const Array<uint4>& data, Amino::MutablePtr<Buffer>& buffer) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");
			HYUU_DECL void write_buffer(MemoryType memory_type, const Array<uchar_t>& data, Amino::MutablePtr<Buffer>& buffer) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");

			HYUU_DECL void read_buffer(const Buffer& buffer, const float_t& type, Amino::bool_t slice, long_t start, long_t length, ArrayPtr<float_t>& data) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");
			HYUU_DECL void read_buffer(const Buffer& buffer, const float2& type, Amino::bool_t slice, long_t start, long_t length, ArrayPtr<float2>& data) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");
			HYUU_DECL void read_buffer(const Buffer& buffer, const float4& type, Amino::bool_t slice, long_t start, long_t length, ArrayPtr<float4>& data) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");
			HYUU_DECL void read_buffer(const Buffer& buffer, const int_t& type, Amino::bool_t slice, long_t start, long_t length, ArrayPtr<int_t>& data) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");
			HYUU_DECL void read_buffer(const Buffer& buffer, const int2& type, Amino::bool_t slice, long_t start, long_t length, ArrayPtr<int2>& data) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");
			HYUU_DECL void read_buffer(const Buffer& buffer, const int4& type, Amino::bool_t slice, long_t start, long_t length, ArrayPtr<int4>& data) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");
			HYUU_DECL void read_buffer(const Buffer& buffer, const uint_t& type, Amino::bool_t slice, long_t start, long_t length, ArrayPtr<uint_t>& data) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");
			HYUU_DECL void read_buffer(const Buffer& buffer, const uint2& type, Amino::bool_t slice, long_t start, long_t length, ArrayPtr<uint2>& data) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");
			HYUU_DECL void read_buffer(const Buffer& buffer, const uint4& type, Amino::bool_t slice, long_t start, long_t length, ArrayPtr<uint4>& data) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");
			HYUU_DECL void read_buffer(const Buffer& buffer, const uchar_t& type, Amino::bool_t slice, long_t start, long_t length, ArrayPtr<uchar_t>& data) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");

			namespace Image {

				class AMINO_ANNOTATE("Amino::Class") HYUU_DECL Image1DBuffer {
				public:
					cl::Image1D m_climage;
					MemoryType m_memory_type;
					int valid = 0;

					Image1DBuffer() {
						m_memory_type = MemoryType::READ_WRITE;
					}

					Image1DBuffer(const Image1DBuffer& input) {
						m_climage = input.m_climage;
						m_memory_type = input.m_memory_type;
						valid = input.valid;
					}

					~Image1DBuffer() = default;
				};

				class AMINO_ANNOTATE("Amino::Class") HYUU_DECL Image2DBuffer {
				public:
					cl::Image2D m_climage;
					MemoryType m_memory_type;
					int valid = 0;

					Image2DBuffer() {
						m_memory_type = MemoryType::READ_WRITE;
					}

					Image2DBuffer(const Image2DBuffer & input) {
						m_memory_type = input.m_memory_type;
						valid = input.valid;
						m_climage = input.m_climage;

						// Proper deep copy of image buffer.
						// The problem is bifrost can't know if it should copy since that depends on the kernel code.
						// For now its just up to the user to copy buffers as needed.
						/*if (input.valid) {
							valid++;
							auto width = input.m_climage.getImageInfo<CL_IMAGE_WIDTH>();
							auto height = input.m_climage.getImageInfo<CL_IMAGE_HEIGHT>();
							auto format = input.m_climage.getImageInfo<CL_IMAGE_FORMAT>();
							auto channel_order = input.m_climage.getImageInfo<CL_IMAGE_FORMAT>().image_channel_order;
							m_climage = cl::Image2D(getContext(), memory_type_to_cl(m_memory_type), cl::ImageFormat(channel_order, CL_FLOAT), width, height);
							getQueue().enqueueCopyImage(input.m_climage, m_climage, {0, 0, 0}, {0, 0, 0}, {width, height, 1});
						}*/
					}

					~Image2DBuffer() = default;
				};

				HYUU_DECL int getCopyCount(const Image2DBuffer& buffer) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");

				class AMINO_ANNOTATE("Amino::Class") HYUU_DECL Image3DBuffer {
				public:
					cl::Image3D m_climage;
					MemoryType m_memory_type;
					int valid = 0;

					Image3DBuffer() {
						m_memory_type = MemoryType::READ_WRITE;
					}

					Image3DBuffer(const Image3DBuffer & input) {
						m_climage = input.m_climage;
						m_memory_type = input.m_memory_type;
						valid = input.valid;
					}

					~Image3DBuffer() = default;
				};

				enum class AMINO_ANNOTATE("Amino::Enum") SamplerBounds : int
				{
					None = 0,  // skip boundary checks, bifrost must have an enum item = 0, this should be replaced by CL_ADDRESS_NONE
					Black = CL_ADDRESS_CLAMP,				// out of bounds returns black
					Clamp = CL_ADDRESS_CLAMP_TO_EDGE,		// clamp coords to image size
					Repeat = CL_ADDRESS_REPEAT,				// normalized coords only!!
					Mirror = CL_ADDRESS_MIRRORED_REPEAT		// normalized coords only!!
				};

				int getSampleBounds(SamplerBounds bounds) {
					if (bounds == SamplerBounds::None)
						return CL_ADDRESS_NONE;
					else
						return static_cast<int>(bounds);
				}

				enum class AMINO_ANNOTATE("Amino::Enum") SamplerInterpolation : int
				{
					Nearest = 0,
					Linear = CL_FILTER_LINEAR
				};

				int getSampleInterpolation(SamplerInterpolation interpolation) {
					if (interpolation == SamplerInterpolation::Nearest)
						return CL_FILTER_NEAREST;
					else
						return static_cast<int>(interpolation);
				}


				struct AMINO_ANNOTATE("Amino::Struct") ImageSampler
				{
					bool normalized_coords;
					SamplerBounds bounds;
					SamplerInterpolation interpolation;
				};


				class AMINO_ANNOTATE("Amino::Class") HYUU_DECL ClSamplerWrapper {
					// NOT intended for use in the graph, the ImageSampler struct should be used instead.
					// This only exists so we can store cl::Sampler in the kernel's args Amino::Any array.
				public:
					cl::Sampler m_clsampler;

					ClSamplerWrapper() = default;

					ClSamplerWrapper(cl::Sampler sampler) {
						m_clsampler = sampler;
					}

					ClSamplerWrapper(const ClSamplerWrapper & input) {
						m_clsampler = input.m_clsampler;
					}
					~ClSamplerWrapper() = default;
				};

				// Allocate image buffer
				HYUU_DECL void allocate_image_buffer(MemoryType memory_type, const uint_t& dimensions, const float_t& type, Amino::MutablePtr<Image1DBuffer>& buffer) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");
				HYUU_DECL void allocate_image_buffer(MemoryType memory_type, const uint_t& dimensions, const float2& type, Amino::MutablePtr<Image1DBuffer>& buffer) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");
				HYUU_DECL void allocate_image_buffer(MemoryType memory_type, const uint_t& dimensions, const float3& type, Amino::MutablePtr<Image1DBuffer>& buffer) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");
				HYUU_DECL void allocate_image_buffer(MemoryType memory_type, const uint_t& dimensions, const float4& type, Amino::MutablePtr<Image1DBuffer>& buffer) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");

				HYUU_DECL void allocate_image_buffer(MemoryType memory_type, const uint2& dimensions, const float_t& type, Amino::MutablePtr<Image2DBuffer>& buffer) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");
				HYUU_DECL void allocate_image_buffer(MemoryType memory_type, const uint2& dimensions, const float2& type, Amino::MutablePtr<Image2DBuffer>& buffer) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");
				HYUU_DECL void allocate_image_buffer(MemoryType memory_type, const uint2& dimensions, const float3& type, Amino::MutablePtr<Image2DBuffer>& buffer) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");
				HYUU_DECL void allocate_image_buffer(MemoryType memory_type, const uint2& dimensions, const float4& type, Amino::MutablePtr<Image2DBuffer>& buffer) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");

				HYUU_DECL void allocate_image_buffer(MemoryType memory_type, const uint3& dimensions, const float_t& type, Amino::MutablePtr<Image3DBuffer>& buffer) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");
				HYUU_DECL void allocate_image_buffer(MemoryType memory_type, const uint3& dimensions, const float2& type, Amino::MutablePtr<Image3DBuffer>& buffer) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");
				HYUU_DECL void allocate_image_buffer(MemoryType memory_type, const uint3& dimensions, const float3& type, Amino::MutablePtr<Image3DBuffer>& buffer) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");
				HYUU_DECL void allocate_image_buffer(MemoryType memory_type, const uint3& dimensions, const float4& type, Amino::MutablePtr<Image3DBuffer>& buffer) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");

				// Write image buffer
				HYUU_DECL void write_image_buffer(MemoryType memory_type, const Array<float_t>& data, const uint_t& dimensions, Amino::MutablePtr<Image1DBuffer>& buffer) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");
				HYUU_DECL void write_image_buffer(MemoryType memory_type, const Array<float2>& data, const uint_t& dimensions, Amino::MutablePtr<Image1DBuffer>& buffer) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");
				HYUU_DECL void write_image_buffer(MemoryType memory_type, const Array<float3>& data, const uint_t& dimensions, Amino::MutablePtr<Image1DBuffer>& buffer) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");
				HYUU_DECL void write_image_buffer(MemoryType memory_type, const Array<float4>& data, const uint_t& dimensions, Amino::MutablePtr<Image1DBuffer>& buffer) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");

				HYUU_DECL void write_image_buffer(MemoryType memory_type, const Array<float_t>& data, const uint2& dimensions, Amino::MutablePtr<Image2DBuffer>& buffer) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");
				HYUU_DECL void write_image_buffer(MemoryType memory_type, const Array<float2>& data, const uint2& dimensions, Amino::MutablePtr<Image2DBuffer>& buffer) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");
				HYUU_DECL void write_image_buffer(MemoryType memory_type, const Array<float3>& data, const uint2& dimensions, Amino::MutablePtr<Image2DBuffer>& buffer) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");
				HYUU_DECL void write_image_buffer(MemoryType memory_type, const Array<float4>& data, const uint2& dimensions, Amino::MutablePtr<Image2DBuffer>& buffer) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");

				HYUU_DECL void write_image_buffer(MemoryType memory_type, const Array<float_t>& data, const uint3& dimensions, Amino::MutablePtr<Image3DBuffer>& buffer) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");
				HYUU_DECL void write_image_buffer(MemoryType memory_type, const Array<float2>& data, const uint3& dimensions, Amino::MutablePtr<Image3DBuffer>& buffer) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");
				HYUU_DECL void write_image_buffer(MemoryType memory_type, const Array<float3>& data, const uint3& dimensions, Amino::MutablePtr<Image3DBuffer>& buffer) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");
				HYUU_DECL void write_image_buffer(MemoryType memory_type, const Array<float4>& data, const uint3& dimensions, Amino::MutablePtr<Image3DBuffer>& buffer) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");

				// Read image buffer
				HYUU_DECL void read_image_buffer(const Image1DBuffer& buffer, const float_t& type, ArrayPtr<float_t>& data, uint_t& dimensions) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");
				HYUU_DECL void read_image_buffer(const Image1DBuffer& buffer, const float2& type, ArrayPtr<float2>& data, uint_t& dimensions) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");
				HYUU_DECL void read_image_buffer(const Image1DBuffer& buffer, const float3& type, ArrayPtr<float3>& data, uint_t& dimensions) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");
				HYUU_DECL void read_image_buffer(const Image1DBuffer& buffer, const float4& type, ArrayPtr<float4>& data, uint_t& dimensions) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");

				HYUU_DECL void read_image_buffer(const Image2DBuffer& buffer, const float_t& type, ArrayPtr<float_t>& data, uint2& dimensions) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");
				HYUU_DECL void read_image_buffer(const Image2DBuffer& buffer, const float2& type, ArrayPtr<float2>& data, uint2& dimensions) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");
				HYUU_DECL void read_image_buffer(const Image2DBuffer& buffer, const float3& type, ArrayPtr<float3>& data, uint2& dimensions) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");
				HYUU_DECL void read_image_buffer(const Image2DBuffer& buffer, const float4& type, ArrayPtr<float4>& data, uint2& dimensions) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");

				HYUU_DECL void read_image_buffer(const Image3DBuffer& buffer, const float_t& type, ArrayPtr<float_t>& data, uint3& dimensions) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");
				HYUU_DECL void read_image_buffer(const Image3DBuffer& buffer, const float2& type, ArrayPtr<float2>& data, uint3& dimensions) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");
				HYUU_DECL void read_image_buffer(const Image3DBuffer& buffer, const float3& type, ArrayPtr<float3>& data, uint3& dimensions) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");
				HYUU_DECL void read_image_buffer(const Image3DBuffer& buffer, const float4& type, ArrayPtr<float4>& data, uint3& dimensions) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");

			}  // namespace Image

			HYUU_DECL void buffer_size_bytes(const Buffer& buffer, Amino::long_t& size) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");
			HYUU_DECL void buffer_size_bytes(const LocalBuffer& buffer, Amino::long_t& size) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");
			HYUU_DECL void buffer_size_bytes(const Image::Image1DBuffer& buffer, Amino::long_t& size) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");
			HYUU_DECL void buffer_size_bytes(const Image::Image2DBuffer& buffer, Amino::long_t& size) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");
			HYUU_DECL void buffer_size_bytes(const Image::Image3DBuffer& buffer, Amino::long_t& size) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");

			HYUU_DECL void copy_buffer(Buffer& target AMINO_ANNOTATE("Amino::InOut outName=buffer_out"), const Buffer& source) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");
			HYUU_DECL void copy_buffer(Image::Image1DBuffer& target AMINO_ANNOTATE("Amino::InOut outName=buffer_out"), const Image::Image1DBuffer& source) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");
			HYUU_DECL void copy_buffer(Image::Image2DBuffer& target AMINO_ANNOTATE("Amino::InOut outName=buffer_out"), const Image::Image2DBuffer& source) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");
			HYUU_DECL void copy_buffer(Image::Image3DBuffer& target AMINO_ANNOTATE("Amino::InOut outName=buffer_out"), const Image::Image3DBuffer& source) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");

		}  // namespace Memory

		namespace Execute {
			class AMINO_ANNOTATE("Amino::Class") HYUU_DECL Program {
			public:
				cl::Program m_clprogram;
				int valid = 0;

				Program() = default;

				Program(const Program & input) {
					m_clprogram = input.m_clprogram;
					valid = input.valid;
				}

				~Program() = default;
			};


			class AMINO_ANNOTATE("Amino::Class") HYUU_DECL Kernel {
			public:
				cl::Kernel m_clkernel;
				Array<Amino::Any> m_args;
				int valid = 0;

				Kernel() {
					m_args = Array<Amino::Any>();
				}

				Kernel(const Kernel & input) {
					valid = input.valid;
					m_args = Array<Amino::Any>(input.m_args);

					if (input.valid) {
						m_clkernel = cl::Kernel(input.m_clkernel);
					}

				}
				~Kernel() = default;
			};

			HYUU_DECL
				void build_program(const Amino::String& source, Amino::MutablePtr<Bifrost::Object>& kernels, Amino::String& log, bool& success) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");

			// Set arg buffer
			HYUU_DECL void set_kernel_arg(Kernel& kernel AMINO_ANNOTATE("Amino::InOut outName=kernel_out"), Amino::long_t arg_id, const Memory::Buffer& value, bool& success) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");
			HYUU_DECL void set_kernel_arg(Kernel& kernel AMINO_ANNOTATE("Amino::InOut outName=kernel_out"), Amino::long_t arg_id, const Memory::LocalBuffer& value, bool& success) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");

			// Set arg image
			HYUU_DECL void set_kernel_arg(Kernel& kernel AMINO_ANNOTATE("Amino::InOut outName=kernel_out"), Amino::long_t arg_id, const Memory::Image::ImageSampler& value, bool& success) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");
			HYUU_DECL void set_kernel_arg(Kernel& kernel AMINO_ANNOTATE("Amino::InOut outName=kernel_out"), Amino::long_t arg_id, const Memory::Image::Image1DBuffer& value, bool& success) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");
			HYUU_DECL void set_kernel_arg(Kernel& kernel AMINO_ANNOTATE("Amino::InOut outName=kernel_out"), Amino::long_t arg_id, const Memory::Image::Image2DBuffer& value, bool& success) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");
			HYUU_DECL void set_kernel_arg(Kernel& kernel AMINO_ANNOTATE("Amino::InOut outName=kernel_out"), Amino::long_t arg_id, const Memory::Image::Image3DBuffer& value, bool& success) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");

			// Set arg scalar
			HYUU_DECL void set_kernel_arg(Kernel& kernel AMINO_ANNOTATE("Amino::InOut outName=kernel_out"), Amino::long_t arg_id, const float_t& value, bool& success) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");
			HYUU_DECL void set_kernel_arg(Kernel& kernel AMINO_ANNOTATE("Amino::InOut outName=kernel_out"), Amino::long_t arg_id, const float2& value, bool& success) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");
			HYUU_DECL void set_kernel_arg(Kernel& kernel AMINO_ANNOTATE("Amino::InOut outName=kernel_out"), Amino::long_t arg_id, const float4& value, bool& success) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");
			HYUU_DECL void set_kernel_arg(Kernel& kernel AMINO_ANNOTATE("Amino::InOut outName=kernel_out"), Amino::long_t arg_id, const int_t& value, bool& success) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");
			HYUU_DECL void set_kernel_arg(Kernel& kernel AMINO_ANNOTATE("Amino::InOut outName=kernel_out"), Amino::long_t arg_id, const int2& value, bool& success) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");
			HYUU_DECL void set_kernel_arg(Kernel& kernel AMINO_ANNOTATE("Amino::InOut outName=kernel_out"), Amino::long_t arg_id, const int4& value, bool& success) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");
			HYUU_DECL void set_kernel_arg(Kernel& kernel AMINO_ANNOTATE("Amino::InOut outName=kernel_out"), Amino::long_t arg_id, const uint_t& value, bool& success) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");
			HYUU_DECL void set_kernel_arg(Kernel& kernel AMINO_ANNOTATE("Amino::InOut outName=kernel_out"), Amino::long_t arg_id, const uint2& value, bool& success) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");
			HYUU_DECL void set_kernel_arg(Kernel& kernel AMINO_ANNOTATE("Amino::InOut outName=kernel_out"), Amino::long_t arg_id, const uint4& value, bool& success) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");
			HYUU_DECL void set_kernel_arg(Kernel& kernel AMINO_ANNOTATE("Amino::InOut outName=kernel_out"), Amino::long_t arg_id, const uchar_t& value, bool& success) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");

			HYUU_DECL
				void get_kernel_args(const Kernel& kernel, ArrayPtr<Amino::Any>& args) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");

			HYUU_DECL
				void execute_kernel(
					Kernel& kernel AMINO_ANNOTATE("Amino::InOut outName=kernel_out"),
					const Array<Amino::ulong_t>& global_offset,
					const Array<Amino::ulong_t>& global_size,
					const Array<Amino::ulong_t>& local_size,
					bool& success,
					Amino::String& error
				) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/opencl_icon.png}]");
		}
	}
}

AMINO_DECLARE_DEFAULT_CLASS(HYUU_DECL, Hyuu::OpenCL::Memory::Buffer);
AMINO_DECLARE_DEFAULT_CLASS(HYUU_DECL, Hyuu::OpenCL::Memory::LocalBuffer);
AMINO_DECLARE_DEFAULT_CLASS(HYUU_DECL, Hyuu::OpenCL::Memory::Image::Image1DBuffer);
AMINO_DECLARE_DEFAULT_CLASS(HYUU_DECL, Hyuu::OpenCL::Memory::Image::Image2DBuffer);
AMINO_DECLARE_DEFAULT_CLASS(HYUU_DECL, Hyuu::OpenCL::Memory::Image::Image3DBuffer);
AMINO_DECLARE_DEFAULT_CLASS(HYUU_DECL, Hyuu::OpenCL::Memory::Image::ImageSampler);
AMINO_DECLARE_DEFAULT_CLASS(HYUU_DECL, Hyuu::OpenCL::Execute::Program);
AMINO_DECLARE_DEFAULT_CLASS(HYUU_DECL, Hyuu::OpenCL::Execute::Kernel);
