#pragma once

#include <functional>
#include <stdexcept>

#include <opencv2/core/utility.hpp>

#include "version.h"

namespace tabread
{
namespace console
{

class Command_line_argument_exception : public std::exception
{
    private:
        std::string _message;

    public:
        Command_line_argument_exception(const char* parameter, const char* message);
        const char* what() const noexcept override;
};

class Command_line_parse_exception : public std::exception
{
    private:
        std::string _message;

    public:
        Command_line_parse_exception(const char* message);
        const char* what() const noexcept override;
};

struct Application_params
{
    Application_params(char** argv_, int argc_) : argv(argv_), argc(argc_) {}

    char** argv;
    int argc;

    int threaded;

    std::string input_path;
    std::string output_path;

#ifdef WIN32
    std::string window_title;
    std::string window_class;

    std::string child_title;
    std::string child_class;
#endif
    std::string bin_path;
};

using Command_line_params = cv::CommandLineParser;

using Fill_params_function = std::function<void(Application_params&, const Command_line_params&)>;
using Check_params_function = std::function<void(Application_params&)>;
using Setup_context_function = std::function<void(const Application_params&)>;

template <typename Fillers, typename Checkers, typename Setups>
bool initialize(const Fillers& fillers, const Checkers& checkers, const Setups& setups, int argc, char** argv)
{
    static const char* keys = "{ help h       || Print help message. }"
                              "{ image i      || Path input image file (get text from image, not the app). }"
	                          "{ output o     || Path to output file. }"
                              "{ threaded th  || Use threaded implementation. }"
                              "{ title t      || Target window's title }"
                              "{ class c      || Target window's class }"
                              "{ ctitle ct    || Target window's widget title }"
                              "{ cclass cc    || Target window's widget class }";

    Application_params params(argv, argc);
    Command_line_params parser(argc, argv, keys);
    parser.about("tabread " TABREAD_VERSION);
    if (parser.has("help"))
    {
        parser.printMessage();
        return false;
    }

    try
    {
        for (const Fill_params_function &fill : fillers)
            fill(params, parser);

        for (const Check_params_function &check : checkers)
            check(params);

        for (const Setup_context_function &setup : setups)
            setup(params);
    }
    catch (...)
    {
        parser.printMessage();
        throw;
    }

    return true;
}

}
}
