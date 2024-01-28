#include <cairo/cairo.h>

#include <CLI/CLI.hpp>

#include "common/StringUtils.h"
#include "common/VRML.h"
#include "function/G4DCM.h"

void test_cairo() {
  cairo_surface_t *surface;
  cairo_t *cr;

  int width = 500, height = 500;

  // Tạo surface để vẽ
  surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
  cr = cairo_create(surface);

  // Màu nền trắng
  cairo_set_source_rgb(cr, 1, 1, 1);
  cairo_paint(cr);

  // Thiết lập màu và độ dày cho đường vẽ
  cairo_set_source_rgb(cr, 0, 0, 0);  // Màu đen
  cairo_set_line_width(cr, 2);

  // Vẽ hình vuông
  cairo_rectangle(cr, 50, 50, 400, 400);
  cairo_stroke(cr);

  // Lưu hình ảnh dưới dạng PNG
  cairo_surface_write_to_png(surface, "square.png");

  // Dọn dẹp
  cairo_destroy(cr);
  cairo_surface_destroy(surface);

  std::cout << "Đã vẽ hình vuông và lưu vào 'square.png'" << std::endl;
}

int main(int argc, char **argv) {
  test_cairo();

  // CLI::App app{"---"};
  // std::string filename = "";
  // app.add_option("-f,--file", filename, "Path to input file");

  // float numberCube = 100;
  // app.add_option("-n,--number", numberCube, "Số lượng cube");

  // float sizeCube = 1;
  // app.add_option("-s,--size", sizeCube, "Size cube");

  // CLI11_PARSE(app, argc, argv);

  // cout << "filename:" << filename << endl;
  // cout << "number:" << numberCube << endl;
  // cout << "size:" << sizeCube << endl;

  // if (filename.empty()) {
  //   return 0;
  // }
  // string outputFilePath = filename + "_" + to_string(int(numberCube)) + "_OUTPUT.wrl";
  // std::ofstream outputFile(outputFilePath, std::ios::out);
  // if (!outputFile.is_open()) {
  //   std::cerr << "Can not write out to file:" << outputFilePath << std::endl;
  //   return 1;
  // }

  // vector<VrmlObject *> vrmlObjects = read_vrml_file(filename);

  // outputFile.close();
  return 1;
}