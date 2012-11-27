
#include "algorithmmodel.h"
#include <normal.hpp>

AlgorithmModel::AlgorithmModel()
{
}

vector<Bee > AlgorithmModel::genesis(const int population, const QSize fieldDims, bool add)
{
    if (!add)
        _bees.clear();

    time_t rawtime;
    time(&rawtime);

    // seed the rand num gen with the current time
    srand(rawtime);

    for (int i = 0; i < population; i++)
    {
        Bee newBee(fieldDims);
        //newBee.setPoint(QPoint(xPos, yPos));
        //cout << "making bee " << i << endl;

        _bees.push_back(newBee);
    }

    return _bees;
}

void AlgorithmModel::computeField(double** foxholes)
{
    ofstream indexFilestream;
    char indexFilename[] = "indices.txt";
    indexFilestream.open(indexFilename, ios::out);

    const int fieldWidth = _fieldDims.width();
    const int fieldHeight = _fieldDims.height();

    double** field = new double*[fieldHeight];

    for (int i = 0; i < fieldHeight; ++i)
    {
        field[i] = new double[fieldWidth];

        for (int j = 0; j < fieldWidth; j++)
            field[i][j] = 0;
    }

    double upperLeftVal;
    double upperRightVal;
    double lowerLeftVal;
    double lowerRightVal;

    int subMatWidth = _fieldDims.width() / (_SHEKEL_DIMENSION - 1);
    int subMatHeight = _fieldDims.height() / (_SHEKEL_DIMENSION - 1);

    float xLeftRatio;
    float yUpRatio;
    float xRightRatio;
    float yDownRatio;

    //double upperXDifference;
    //double lowerXDifference;

    double weightedUpperLeftValue;
    double weightedLowerLeftValue;
    double weightedUpperRightValue;
    double weightedLowerRightValue;

    //double yDifference;

    int iIdx;
    int jIdx;

    for (int k = 1; k < _SHEKEL_DIMENSION; k++)
    {
        for (int l = 1; l < _SHEKEL_DIMENSION; l++)
        {
            upperLeftVal = foxholes[k - 1][l - 1];
            upperRightVal = foxholes[k - 1][l];
            lowerLeftVal = foxholes[k][l - 1];
            lowerRightVal = foxholes[k][l];

            //upperXDifference = upperRightVal - upperLeftVal;
            //lowerXDifference = lowerRightVal - lowerLeftVal;

            for (int i = 0; i < subMatHeight; i++)
            {
                yDownRatio = (float) i / subMatHeight;
                yUpRatio = 1 - yDownRatio;

                for (int j = 0; j < subMatWidth; j++)
                {
                    xRightRatio = (float) j / subMatWidth;
                    xLeftRatio = 1 - xRightRatio;

                    weightedUpperLeftValue = upperLeftVal * yUpRatio * xLeftRatio;
                    weightedLowerLeftValue = lowerLeftVal * xLeftRatio * yDownRatio;
                    weightedUpperRightValue = upperRightVal * yUpRatio * xRightRatio;
                    weightedLowerRightValue = lowerRightVal * yDownRatio * xRightRatio;

                    //yDifference = weightedLowerValue - weightedUpperValue;

                    iIdx = (k - 1) * subMatHeight + i;
                    jIdx = (l - 1) * subMatWidth + j;

                    //field[iIdx][jIdx]  = weightedLowerValue + yDifference * yRatio;
                    field[iIdx][jIdx]  = weightedUpperLeftValue + weightedLowerLeftValue
                                         + weightedUpperRightValue + weightedLowerRightValue;
                }
            }
        }
    }

    _field =  const_cast<const double**>(field);

    ofstream fieldFilestream;
    char fieldFilename[] = "field.txt";
    fieldFilestream.open(fieldFilename, ios::out);

    for (int i = 0; i < fieldHeight; i++)
    {
        for (int j = 0; j < fieldWidth; j++)
        {
            fieldFilestream << _field[i][j] << " ";
        }
        fieldFilestream << endl;
    }
    fieldFilestream.close();
}

const double** AlgorithmModel::getField()
{
    return  _field;
}

void AlgorithmModel::setHive(const QSize fieldDims)
{
    _fieldDims = fieldDims;
    _hive = Hive(fieldDims);
}

Hive AlgorithmModel::getHive()
{
    return _hive;
}

void AlgorithmModel::setBees(vector<Bee > bees)
{
    this->_bees = bees;
}

const vector<Bee > AlgorithmModel::getBees()
{
    cout << "returning bee vector size " << _bees.size() << endl;
    return  _bees;
}

double** AlgorithmModel::foxholes()
{
    //function y = foxholes(x, noPause)

    /*
        %FOXHOLES  Evaluate Shekel's Foxholes function.
        %       Y = FOXHOLES(X) with a two-element input vector X evaluates Shekel's
        %       Foxholes function at X. The elements of X have to be from the interval
        %       [-65.536, 65.536]. The global minimum of this function is about
        %       0.998004 at X = [-32, -32].
        %
        %       FOXHOLES (without input arguments) displays the function as a contour
        %       plot.
        %
        %       Markus Buehren
        %       Last modified 03.02.2008
    */

    const int testMatDimension = 10;

    double** F = new double*[testMatDimension];

    for (int i = 0; i < testMatDimension; ++i)
    {
        F[i] = new double[testMatDimension];

        for (int j = 0; j < testMatDimension; j++)
            F[i][j] = 0;
    }

    int x [testMatDimension];

    for (int idx = 0; idx < testMatDimension; idx++)
        x[idx] = idx;

    time_t rawtime;
    time(&rawtime);

    // seed the rand num gen with the current time
    srand(rawtime);

    for (int m = 0; m < testMatDimension; m++)
        for (int n = 0; n < testMatDimension; n++)
        {
            int constants [2];
            constants[0] = x[m];
            constants[1] = x[n];
            F[m][n] = foxHelper(constants);
        }
    return F;
}

double AlgorithmModel::foxHelper(int* x)
{
    //% compute function value

    float sum = 0.0;

    //%there appears to be no effect
    //%holessqrt = 5;

    int seed = rand();
    float normal;
    float h;

    for (int i = 0; i < 30; i++)
    {
        float sp = 0.0;

        for (int j = 0; j < x[0]; j++)
        {
            normal = r4_normal((float) 5.0, (float) 2.0, seed);
            h = x[1] - normal;
            //% h = x(2) - a(i, j);
            sp = sum + h * h;
        }
        normal = r4_normal(0.5, 0.2, seed);
        sum = sum - 1.0 / (sp + normal);
    }

    return -sum;

    //if (nargin == 1 || ~noPause)
    //    pause(0.05);
}
