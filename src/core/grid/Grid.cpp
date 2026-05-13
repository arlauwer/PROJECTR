#include "Grid.hpp"
#include "RadiationField.hpp"

void Grid::finalize()
{
    if (_radField)
    {
        RadiationField& rad = radiationField();

        // convert Lds to mean intensity
        for (size_t wavIndex; wavIndex < rad.numBins; wavIndex++)
        {
            real wavWidth = rad.wavGrid.width(wavIndex);
            for (size_t cellIndex = 0; cellIndex < numCells(); cellIndex++)
            {
                real L                   = rad(cellIndex, wavIndex);
                rad(cellIndex, wavIndex) = L / (4 * M_PI * wavWidth * volume(cellIndex));
            }
        }
    }
}
