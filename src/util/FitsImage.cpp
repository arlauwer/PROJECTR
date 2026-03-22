#include "FitsImage.hpp"
#include "fitsio/fitsio.h"
#include <stdexcept>

namespace
{

void check(int status, const string& filepath, const string& action)
{
    if (!status)
        return;
    char message[FLEN_STATUS];
    ffgerr(status, message);
    throw std::runtime_error("Error " + action + " FITS file " + filepath + ": " + message);
}

} // namespace

FitsImage::FitsImage(size_t nx, size_t ny, size_t nz)
    : _nx(nx),
      _ny(ny),
      _nz(nz),
      _data(nx * ny * nz, 0.0)
{
}

void FitsImage::save(const string& filepath) const
{
    int       status = 0;
    fitsfile* fptr;

    remove(filepath.c_str());

    ffdkinit(&fptr, filepath.c_str(), &status);
    check(status, filepath, "creating");

    long naxes[3] = {(long)_nx, (long)_ny, (long)_nz};
    int  naxis    = (_nz > 1) ? 3 : 2;
    ffcrim(fptr, DOUBLE_IMG, naxis, naxes, &status);
    check(status, filepath, "creating image");

    ffpprd(fptr, 0, 1, _data.size(), const_cast<double*>(_data.data()), &status);
    check(status, filepath, "writing");

    ffclos(fptr, &status);
    check(status, filepath, "closing");
}