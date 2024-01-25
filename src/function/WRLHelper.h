#ifndef FUNCTION_WRLHELPER_H_
#define FUNCTION_WRLHELPER_H_

#include <sstream>

#include "../common/Model.h"
#include "../common/StringUtils.h"

using namespace std;

class WrlShapeCubic {
public:
  //   Color diffuseColor;
  string cmd = "";
  Point midPoint;
  Size3D size = Size3D(0.5);
  string name = "";
  WrlShapeCubic(string cmd, Point midPoint, string name) : cmd(cmd), midPoint(midPoint), name(name) {}

  string print() {
    ostringstream lines;
    lines << "#---------- SOLID: cubic: " << name << endl;
    lines << "Shape {" << endl;
    lines << "	appearance Appearance {" << endl;
    lines << "		material Material {" << endl;

    lines << colorLine(cmd) << endl;
    // lines << "				diffuseColor 1 1 1" << endl;

    lines << "			transparency 0.7" << endl;
    lines << "		}" << endl;
    lines << "	}" << endl;
    lines << "	geometry IndexedFaceSet {" << endl;
    lines << "		coord Coordinate {" << endl;
    lines << "			point [" << endl;

    lines << pointLines();
    // lines << "					-300 -100 -100," << endl;
    // lines << "					300 -100 -100," << endl;
    // lines << "					300 100 -100," << endl;
    // lines << "					-300 100 -100," << endl;
    // lines << "					-300 -100 100," << endl;
    // lines << "					300 -100 100," << endl;
    // lines << "					300 100 100," << endl;
    // lines << "					-300 100 100," << endl;

    lines << "			]" << endl;
    lines << "		}" << endl;
    lines << "		coordIndex [" << endl;
    lines << "			0, 3, 2, 1, -1," << endl;
    lines << "			4, 7, 3, 0, -1," << endl;
    lines << "			7, 6, 2, 3, -1," << endl;
    lines << "			6, 5, 1, 2, -1," << endl;
    lines << "			5, 4, 0, 1, -1," << endl;
    lines << "			4, 5, 6, 7, -1," << endl;
    lines << "		]" << endl;
    lines << "		solid FALSE" << endl;
    lines << "	}" << endl;
    lines << "}" << endl;

    return lines.str();
  }

private:
  Size3D haftSize() {
    float haftWidth = size.width / 2;
    float haftHeight = size.height / 2;
    float haftDepth = size.depth / 2;
    return Size3D(haftWidth, haftHeight, haftDepth);
  };

  string colorLine(string cmd) {
    string line = "			diffuseColor ";
    string lower = toLowerCase(cmd);
    if (lower == toLowerCase("initStep")) {
      line += "1 1 1";
    } else if (lower == toLowerCase("Transportation")) {
      line += "0 0 0";
    } else if (lower == toLowerCase("phot")) {
      line += "0 1 0";
    } else if (lower == toLowerCase("eIoni")) {
      line += "0 0 1";
    } else if (lower == toLowerCase("compt")) {
      line += "1 0 0";
    } else if (lower == toLowerCase("Rayl")) {
      line += "1 0.9 0";
    } else if (lower == toLowerCase("CoulumbScat")) {
      line += "0.8 0 0.7";
    } else if (lower == toLowerCase("Msc")) {
      line += "0.6 0.3 0";
    } else if (lower == toLowerCase("eBrem")) {
      line += "1 0.4 0";
    } else if (lower == toLowerCase("annihil")) {
      line += "0.6 0.1 1";
    } else {
      line += "1 1 1";
    }
    line += "  # " + cmd;
    return line;
  }

  string pointLines() {
    Size3D s = haftSize();
    Point p = midPoint;
    ostringstream stream;
    stream << "				" << p.x - s.width << " " << p.y - s.height << " " << p.z - s.depth << "," << endl;  // 1
    stream << "				" << p.x + s.width << " " << p.y - s.height << " " << p.z - s.depth << "," << endl;  // 2
    stream << "				" << p.x + s.width << " " << p.y + s.height << " " << p.z - s.depth << "," << endl;  // 3
    stream << "				" << p.x - s.width << " " << p.y + s.height << " " << p.z - s.depth << "," << endl;  // 4
    stream << "				" << p.x - s.width << " " << p.y - s.height << " " << p.z + s.depth << "," << endl;  // 5
    stream << "				" << p.x + s.width << " " << p.y - s.height << " " << p.z + s.depth << "," << endl;  // 6
    stream << "				" << p.x + s.width << " " << p.y + s.height << " " << p.z + s.depth << "," << endl;  // 7
    stream << "				" << p.x - s.width << " " << p.y + s.height << " " << p.z + s.depth << "," << endl;  // 8
    return stream.str();
  };
};

#endif  // FUNCTION_WRLHELPER_H_