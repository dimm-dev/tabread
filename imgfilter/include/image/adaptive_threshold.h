#pragma once

#include "image_filter.h"

namespace tabread
{
namespace imgproc
{

struct Theshold_params
{
    int max_value;
    int method;
    int type;
    int block_size;
    int substract_c;
};

extern const Theshold_params Theshold_params_default;

class Adaptive_threshold_filter : public Image_filter
{
    public:
        Adaptive_threshold_filter(const Theshold_params& params = Theshold_params_default);
        void execute(cv::Mat& dst) override;

    private:
        Theshold_params _params;
};

}
}
