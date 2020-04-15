#include <optional>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <io2d.h>
#include "route_model.h"
#include "render.h"
#include "route_planner.h"

using namespace std::experimental;

static std::optional<std::vector<std::byte>> ReadFile(const std::string &path)
{   
    std::ifstream is{path, std::ios::binary | std::ios::ate};
    if( !is )
        return std::nullopt;
    
    auto size = is.tellg();
    std::vector<std::byte> contents(size);    
    
    is.seekg(0);
    is.read((char*)contents.data(), size);

    if( contents.empty() )
        return std::nullopt;
    return std::move(contents);
}

void ReadUserInput(float &startx, float &starty, float &endx, float &endy){
    std::cout << "\n\n Welcome to the Path Planning App" << std::endl;
    std::cout << "Enter a number between 0-100 for start_x: ";
    std::cin >> startx;
    while( !std::cin && startx< 0 && startx >100){
        std::cout << "Enter a valid number between 0-100 for start_x: " << std:: endl;
        std::cin.clear();
        std::cin >> startx;
    }

    std::cout << "Enter a number between 0-100 for start_y: ";
    std::cin >> starty;
    while( !std::cin && starty< 0 && starty >100){
        std::cout << "Enter a valid number between 0-100 for start_y: " << std:: endl;
        std::cin.clear();
        std::cin >> starty;
    }

    std::cout << "Enter a number between 0-100 for end_x: ";
    std::cin >> endx;
    while( !std::cin && endx< 0 && endx >100){
        std::cout << "Enter a valid number between 0-100 for end_x: " << std:: endl;
        std::cin.clear();
        std::cin >> endx;
    }

    std::cout << "Enter a number between 0-100 for end_y: ";
    std::cin >> endy;
    while( !std::cin && endy< 0 && endy >100){
        std::cout << "Enter a valid number between 0-100 for end_y: " << std:: endl;
        std::cin.clear();
        std::cin >> endy;
    }
}

int main(int argc, const char **argv)
{    
    std::string osm_data_file = "";
    if( argc > 1 ) {
        for( int i = 1; i < argc; ++i )
            if( std::string_view{argv[i]} == "-f" && ++i < argc )
                osm_data_file = argv[i];
    }
    else {
        std::cout << "To specify a map file use the following format: " << std::endl;
        std::cout << "Usage: [executable] [-f filename.osm]" << std::endl;
        osm_data_file = "../map.osm";
    }
    
    std::vector<std::byte> osm_data;
 
    if( osm_data.empty() && !osm_data_file.empty() ) {
        std::cout << "Reading OpenStreetMap data from the following file: " <<  osm_data_file << std::endl;
        auto data = ReadFile(osm_data_file);
        if( !data )
            std::cout << "Failed to read." << std::endl;
        else
            osm_data = std::move(*data);
    }
    
    // TODO 1: Declare floats `start_x`, `start_y`, `end_x`, and `end_y` and get
    // user input for these values using std::cin. Pass the user input to the
    // RoutePlanner object below in place of 10, 10, 90, 90.

    // Build Model.
    RouteModel model{osm_data};
	
	float start_x, start_y, end_x, end_y;
	ReadUserInput(start_x, start_y, end_x, end_y);
	
    // Create RoutePlanner object and perform A* search.
    RoutePlanner route_planner{model, start_x, start_y, end_x, end_y};
    route_planner.AStarSearch();

    std::cout << "Distance: " << route_planner.GetDistance() << " meters. \n";

    // Render results of search.
    Render render{model};

    auto display = io2d::output_surface{400, 400, io2d::format::argb32, io2d::scaling::none, io2d::refresh_style::fixed, 30};
    display.size_change_callback([](io2d::output_surface& surface){
        surface.dimensions(surface.display_dimensions());
    });
    display.draw_callback([&](io2d::output_surface& surface){
        render.Display(surface);
    });
    display.begin_show();
}
