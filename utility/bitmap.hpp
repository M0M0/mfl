#include <cstdint>
#include <cstring>
#include <cassert>

#include <png.h>

typedef struct {
  std::uint8_t red;
  std::uint8_t green;
  std::uint8_t blue;
} pixel_t;

// dependency to libpng!
class Bitmap {
 public:
  Bitmap(size_t width, size_t height) : width_(width), height_(height) {
    field_ = new pixel_t[width_ * height_];
    std::memset(field_,0,width_ * height_ * 3);
  }
  ~Bitmap() {
    delete[] field_;
  }

  // some proper errorchecking might be nessary
  int Set(size_t x, size_t y, std::uint8_t red, std::uint8_t green,
	  std::uint8_t blue) {
    pixel_t * ptr = _Pixel(x,y);
    assert(red < 256);
    assert(green < 256);
    assert(blue < 256);
    ptr->red = red;
    ptr->green = green;
    ptr->blue = blue;
    return 0;
  }
  int Write(char const * path) {
    FILE * fileptr;
    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    png_byte ** row_pointers = NULL;
    
    const size_t pixel_size = 3;
    const size_t depth = 8;

    fileptr = fopen(path,"wb");
    if (!fileptr) return -1;

    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING,NULL,NULL,NULL);
    if (png_ptr == NULL) {
      fclose(fileptr);
      return -1;
    }
    
    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
      png_destroy_write_struct(&png_ptr,&info_ptr);
      fclose(fileptr);
      return status;
    }
    
    if (setjmp(png_jmpbuf(png_ptr))) {
      png_destroy_write_struct(&png_ptr,&info_ptr);
      fclose(fileptr);
      return -1;
    }
    
    png_set_IHDR(png_ptr,
		 info_ptr,
		 width_,
		 height_,
		 depth,
		 PNG_COLOR_TYPE_RGB,
		 PNG_INTERLACE_NONE,
		 PNG_COMPRESSION_TYPE_DEFAULT,
		 PNG_FILTER_TYPE_DEFAULT);
    row_pointers = reinterpret_cast<png_byte**>(
		     png_malloc(png_ptr,height_ * sizeof(png_byte*)) );
    png_byte * row;
    pixel_t * pixel;
    for (int y = 0; y < height_; ++y) {
      row = reinterpret_cast<png_byte*>( 
	      png_malloc(png_ptr,sizeof(std::uint8_t) * width_ * pixel_size) );
      row_pointers[y] = row;
      for (int x = 0; x < width_; ++x) {
	pixel = _Pixel(x,y);
	*row++ = pixel->red;
	*row++ = pixel->green;
	*row++ = pixel->blue;
      }
    }
    
    png_init_io(png_ptr,fileptr);
    png_set_rows(png_ptr,info_ptr,row_pointers);
    png_write_png(png_ptr,info_ptr,PNG_TRANSFORM_IDENTITY,NULL);
    
    for (int y = 0; y < height_; ++y) png_free(png_ptr,row_pointers[y]);
    png_free(png_ptr,row_pointers);
    
    png_destroy_write_struct(&png_ptr,&info_ptr);
    fclose(fileptr);

    return 0;
  }

 private:
  // get the address of individual pixels
  pixel_t * _Pixel(size_t x, size_t y) {
    assert(x < width_);
    assert(y < height_);
    return field_ + width_ * y + x;
  }

  pixel_t * field_;
  size_t width_;
  size_t height_;
};
