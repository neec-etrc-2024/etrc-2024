#pragma ones

#include "include/interface/Run.hpp"

class line_tracer  : public Run
{
private:
    /* data */
public:
    void run() override;
    line_tracer(/* args */);
    ~line_tracer();
};

line_tracer::line_tracer(/* args */)
{
}

line_tracer::~line_tracer()
{
}

