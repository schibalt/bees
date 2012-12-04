#include "simulateseason.h"

WorkerBee::WorkerBee(QObject* parent) :
    QObject(parent)
{
}

void WorkerBee::disconnectEverything(QThread& thread)
{
    //colony/hive generation
    disconnect(&thread, SIGNAL(started()), this, SLOT(generate()));
    disconnect(this, SIGNAL(quitBeeGenThread()), &thread, SLOT(quit()));

    //field generation
    disconnect(&thread, SIGNAL(started()), this, SLOT(foxholes()));
    disconnect(this, SIGNAL(quitFieldGenThread()), &thread, SLOT(quit()));

    //evaluate fitnesses
    disconnect(&thread, SIGNAL(started()), this, SLOT(evaluateFitnesses()));
    disconnect(this, SIGNAL(quitFitEvalThread()), &thread, SLOT(quit()));

    //select sites
    disconnect(&thread, SIGNAL(started()), this, SLOT(selectSites()));
    disconnect(this, SIGNAL(quitSiteSelectThread()), &thread, SLOT(quit()));

    //recruit bees
    disconnect(&thread, SIGNAL(started()), this, SLOT(recruit()));
    disconnect(this, SIGNAL(quitRecruitmentThread()), &thread, SLOT(quit()));

    //evaluate fitnesses of bees in each neighborhood
    disconnect(&thread, SIGNAL(started()), this, SLOT(evalNeighborFits()));
    disconnect(this, SIGNAL(quitNeighborFitEvalThread()), &thread, SLOT(quit()));

    //new gen
    disconnect(&thread, SIGNAL(started()), this, SLOT(regenerate()));
    disconnect(this, SIGNAL(quitRegenThread()), &thread, SLOT(quit()));
}

void WorkerBee::setGenesisMembers(
    QThread& thread,
    int population,
    QSize fieldDims,
    bool add
)
{
    disconnectEverything(thread);

    //when the thread it started generate the bees
    connect(&thread, SIGNAL(started()), SLOT(generate()), Qt::UniqueConnection);
    connect(this, SIGNAL(quitBeeGenThread()), &thread, SLOT(quit()), Qt::UniqueConnection);

    _population = population;
    _fieldDims = fieldDims;
    _add = add;
}

void WorkerBee::setHive()
{
    _hive = Hive(_fieldDims);
}

Hive WorkerBee::getHive()
{
    return _hive;
}

const double** WorkerBee::getFoxholes()
{
    return  const_cast<const double**>(_foxholes);
}

const double** WorkerBee::getField()
{
    return  const_cast<const double**>(_field);
}

void WorkerBee::generate()
{
    if (!_add)
        _bees.clear();

    time_t rawtime;
    time(&rawtime);

    // seed the rand num gen with the current time
    srand(rawtime);

    for (int i = 0; i < _population; i++)
    {
        Bee newBee(_fieldDims);
        //newBee.setPoint(QPoint(xPos, yPos));
        //qDebug() << "making bee " << i;

        _bees.push_back(newBee);
    }

    setHive();

    emit quitBeeGenThread();
    emit beesGenerated();
}

const vector<Bee >* WorkerBee::getBees()
{
    return const_cast<const vector<Bee >* >(&_bees);
}

void WorkerBee::setFieldGenMembers(
    QThread& thread,
    int foxholes,
    int maxima,
    int bound,
    int power,
    bool deterministic,
    bool d1,
    bool d2
)
{
    disconnectEverything(thread);

    //when the thread it started generate the bees
    connect(&thread, SIGNAL(started()), SLOT(foxholes()), Qt::UniqueConnection);
    connect(this, SIGNAL(quitFieldGenThread()), &thread, SLOT(quit()), Qt::UniqueConnection);

    _foxholeParam = foxholes;
    _maxima = maxima;
    _bound = bound;
    _power = power;
    _deterministic = deterministic;
    _d1 = d1;
    _d2 = d2;
}

void WorkerBee::foxholes()
{
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

    const int matdim = (2 * _foxholeParam) + 1;

    //inst/init result matrix
    _foxholes = new double*[matdim];

    for (int i = 0; i < matdim; ++i)
        _foxholes[i] = new double[matdim];

    int x [matdim];
    qDebug() << "the shekel array has " << matdim << " elements";

    int value;

    //init array (default -10:1:10)
    for (int idx = 0; idx < matdim; idx++)
    {
        value = idx - _foxholeParam;
        //qDebug() << 'x' << idx << " is " << value;
        x[idx] = value;
    }

    time_t rawtime;
    time(&rawtime);

    // seed the rand num gen with the current time
    srand(rawtime);

    //double foxhole;
    int xpair [2];

    for (int m = 0; m < matdim; m++)
        for (int n = 0; n < matdim; n++)
        {
            xpair[0] = x[m];
            xpair[1] = x[n];

            _foxholes[m][n] = foxHelper(xpair);
        }
    emit foxholesGenerated();
}

double WorkerBee::foxHelper(int* x)
{
    const int maximaSqrt = sqrt(_maxima);

    double constA [_maxima][2];
    double a0 [maximaSqrt];

    int maxSqrtHalf = maximaSqrt / 2;

    for (int constIdx = 0; constIdx < maximaSqrt; constIdx++)
        a0[constIdx] = ((constIdx - maxSqrtHalf) * 16);

    for (int idx = 0; idx < maximaSqrt; idx++)
        for (int subidx = 0; subidx < maximaSqrt; subidx++)
        {
            int cell = (maximaSqrt * idx) + subidx;
            constA[cell][0] = a0[subidx];
            constA[cell][1] = a0[idx];
        }

    float tmp = 0.0;
    float tmp2;

    double aval;
    int seed = rand();
    float uniform;
    const int matdim = (2 * _foxholeParam) + 1;

    for (int j = 0; j < _maxima; j++)
    {
        tmp2 = 0.0;

        for (int i = 0; i < _DIMENSIONS; i++)
        {
            uniform = r4_uniform_01(seed);
            //qDebug() << "uniform is " << uniform;

            if (_deterministic)
                aval = constA[j][i];
            else
                aval = (uniform * matdim) - _foxholeParam;

            //qDebug() << "aval is " << aval;
            tmp2 += pow(x[i] - aval, _power);
        }
        uniform = _maxima * r4_uniform_01(seed);
        // qDebug() << "uniform is " << uniform;

        if (_deterministic)
            tmp += (1 / ((j + 1) + tmp2));
        else
            tmp += (1 / (tmp2 + uniform));
    }

    if (_d1)
        return -(1 / (.002 + tmp));
    else
        return tmp * 10;
    /*
    if (_deterministic)
    {

        return 119.998 - tmp;
        return .002 + tmp;
        return (1 / (.002 + tmp));
    }
    else
    {
        return tmp * 10;
        return tmp;
        return tmp * 10;
        return .002 + tmp;
    }
    */
}

void WorkerBee::computeField()
{
    ofstream indexFilestream;
    char indexFilename[] = "indices.txt";
    indexFilestream.open(indexFilename, ios::out);

    const int fieldWidth = _fieldDims.width();
    const int fieldHeight = _fieldDims.height();

    //double** field = new double*[fieldHeight];
    _field = new double*[fieldHeight];

    for (int i = 0; i < fieldHeight; ++i)
    {
        _field[i] = new double[fieldWidth];

        for (int j = 0; j < fieldWidth; j++)
            _field[i][j] = 0;
    }

    double upperLeftVal;
    double upperRightVal;
    double lowerLeftVal;
    double lowerRightVal;

    const int shekelArraySize = (2 * _foxholeParam) + 1;

    double subMatWidth = (double) _fieldDims.width() / (shekelArraySize - 1);
    double subMatHeight = (double) _fieldDims.height() / (shekelArraySize - 1);

    float xLeftRatio;
    float yUpRatio;
    float xRightRatio;
    float yDownRatio;

    double weightedUpperLeftValue;
    double weightedLowerLeftValue;
    double weightedUpperRightValue;
    double weightedLowerRightValue;

    int iIdx;
    int jIdx;

    for (int k = 1; k < shekelArraySize; k++)
        for (int l = 1; l < shekelArraySize; l++)
        {
            upperLeftVal = _foxholes[k - 1][l - 1];
            upperRightVal = _foxholes[k - 1][l];
            lowerLeftVal = _foxholes[k][l - 1];
            lowerRightVal = _foxholes[k][l];

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

                    iIdx = ((k - 1) * subMatHeight) + i;
                    jIdx = ((l - 1) * subMatWidth) + j;

                    _field[iIdx][jIdx]  = weightedUpperLeftValue + weightedLowerLeftValue
                                          + weightedUpperRightValue + weightedLowerRightValue;
                }
            }
        }

    //_field =  const_cast<const double**>(field);

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

    emit quitFieldGenThread();
    emit fieldGenerated();
}

void WorkerBee::setFitnessEvalMembers(QThread& thread, double distancePenalty)
{
    _distancePenalty = distancePenalty;
    disconnectEverything(thread);

    //when the thread it started generate the bees
    connect(&thread, SIGNAL(started()), this, SLOT(evaluateFitnesses()), Qt::UniqueConnection);
    connect(this, SIGNAL(quitFitEvalThread()), &thread, SLOT(quit()), Qt::UniqueConnection);
}

void WorkerBee::evaluateFitnesses()
{
    // qDebug() << "evaluating fitnesses";
    int beeJ;
    int beeI;

    int hiveJ = _hive.getPoint().x();
    int hiveI = _hive.getPoint().y();

    double fieldPtQuality;
    double distance;
    double fitness;
    int inc = 0;
    Bee* bee;

    for (vector<Bee >::iterator i = _bees.begin(); i != _bees.end(); ++i)
    {
        bee = &(*i);
        beeJ = bee->getPoint().x();
        beeI = bee->getPoint().y();
        distance = sqrt(pow(beeJ - hiveJ, 2) + pow(beeI - hiveI, 2));
        fieldPtQuality = _field[beeI][beeJ];
qDebug() << "distance penalty is " << _distancePenalty;
        double weightedDistance = _distancePenalty * distance;
        qDebug() << "weightedDistance penalty is " << weightedDistance;

        if (weightedDistance < 1)
            weightedDistance = 1;

        fitness = fieldPtQuality * weightedDistance;
        //qDebug() << "bee " << inc << " fitness is " << fitness;

        bee->setFitness(fitness);
        ++inc;
    }
    sort(_bees.begin(), _bees.end());
    reverse(_bees.begin(), _bees.end());

    /*
    inc = 0;

    foreach(Bee bee, _bees)
    {
        qDebug() << "bee " << inc << " fitness is " << bee.getFitness();

        ++inc;
    }
    */

    emit quitFitEvalThread();
    emit fitnessesEvaluated();
}

void WorkerBee::setSiteSelectionMembers(QThread& thread, int sites, int eliteSites,
                                        double deltaLambda,
                                        double deltaPhi)
{
    _deltaLambda = deltaLambda;
    _deltaPhi = deltaPhi;
    disconnectEverything(thread);

    connect(&thread, SIGNAL(started()), this, SLOT(selectSites()), Qt::UniqueConnection);
    connect(this, SIGNAL(quitSiteSelectThread()), &thread, SLOT(quit()), Qt::UniqueConnection);

    _sites = sites;
    _eliteSites = eliteSites;
}

void WorkerBee::selectSites()
{
    _priorityBees.clear();
    _eliteBees.clear();

    Bee* bee;

    int fieldWidth = _fieldDims.width();
    int fieldHeight = _fieldDims.height();

    int deltaLambda = fieldWidth * _deltaLambda;
    deltaLambda /= 100;
    int deltaPhi = fieldHeight * _deltaPhi;
    deltaPhi /= 100;

    int prevBeeX ;
    int prevBeeY;

    unsigned int inc = 0;

    while (_eliteBees.size() < _eliteSites)
    {
        bee = &_bees[inc];

        if (_eliteBees.size() > 0 || inc >= _bees.size() - _sites + _eliteBees.size())
        {
            prevBeeX = _eliteBees.back()->getPoint().x();
            prevBeeY = _eliteBees.back()->getPoint().y();

            if (bee->getPoint().x() < prevBeeX + deltaLambda
                && bee->getPoint().x() > prevBeeX - deltaLambda
                && bee->getPoint().y() > prevBeeY - deltaPhi
                && bee->getPoint().y() < prevBeeY  + deltaPhi)
            {
                ++inc;
                continue;
            }
            else
            {
                bee->setRole(Bee::ELITE);
                _eliteBees.push_back(bee);
            }
        }
        else
        {
            bee->setRole(Bee::ELITE);
            _eliteBees.push_back(bee);
        }
        ++inc;
        //    qDebug() << "bee with fitness " << (*i).getFitness() << " is at an elite site";
    }

    while (_priorityBees.size() < _sites - _eliteSites)
    {
        bee = &_bees[inc];

        if (_priorityBees.size() > 0
            || inc >= _bees.size() - _sites + (_eliteBees.size() + _priorityBees.size()))
        {
            prevBeeX = _priorityBees.back()->getPoint().x();
            prevBeeY = _priorityBees.back()->getPoint().y();

            if (bee->getPoint().x() < prevBeeX + deltaLambda
                && bee->getPoint().x() > prevBeeX - deltaLambda
                && bee->getPoint().y() > prevBeeY - deltaPhi
                && bee->getPoint().y() < prevBeeY  + deltaPhi)
            {
                ++inc;
                continue;
            }
            else
            {
                bee->setRole(Bee::PRIORITY);
                _priorityBees.push_back(bee);
            }
        }
        else
        {
            bee->setRole(Bee::PRIORITY);
            _priorityBees.push_back(bee);
        }
        ++inc;
        //    qDebug() << "bee with fitness " << (*i).getFitness() << " is at a priority site";
    }
    emit quitSiteSelectThread();
    emit sitesSelected();
}

void WorkerBee::setRecruitmentMembers
(
    QThread& thread,
    double randomCut,
    double deltaLambda,
    double deltaPhi
)
{
    disconnectEverything(thread);

    _randomCut = randomCut;
    _deltaLambda = deltaLambda;
    _deltaPhi = deltaPhi;
    _eliteWeight = 2;
    _priorityWeight = 1;

    connect(this, SIGNAL(quitRecruitmentThread()), &thread, SLOT(quit()), Qt::UniqueConnection);
    connect(&thread, SIGNAL(started()), this, SLOT(recruit()), Qt::UniqueConnection);
}

void WorkerBee::recruit()
{
    //convert to raiot 0:1
    double scoutRatio = _randomCut / 100;

    //set the actual number of scouts
    int scouts = _bees.size() * scoutRatio;

    int elites = _eliteBees.size();
    int prioritized = _priorityBees.size();

    //recruits + priority bees + elite bees
    int remainder = _bees.size() /*- scouts*/ - elites - prioritized;

    //weight the numbers of bees at pri. + elite sites
    double eliteWeight = elites * _eliteWeight;
    double priorityWeight = prioritized * _priorityWeight;

    //set ratios 0:1
    double eliteRatio = (eliteWeight / (eliteWeight + priorityWeight));
    double priorityRatio = 1 - eliteRatio;

    //set bees per site type
    int eliteRecruits = remainder * eliteRatio;
    int priorityRecruits = remainder * priorityRatio;

    //set bees per site
    int perPrioritySite = priorityRecruits / prioritized;
    int perEliteSite = eliteRecruits / elites;

    //account for remainder
    int extraElites = eliteRecruits - (elites * perEliteSite);
    int extraPrioritized = priorityRecruits - (prioritized * perPrioritySite);

    //add to scout pool
    scouts += extraElites;
    scouts += extraPrioritized;

    //seems unecessary
    //perEliteSite -= extraElites;
    //perPrioritySite -= extraPrioritized;

    int bee = _bees.size();

    _eliteNeighborhoods.clear();
    _priorityNeighborhoods.clear();
    Bee* neighborBee;

    for (int eliteHood = 0; eliteHood < elites; eliteHood++)
    {
        vector<Bee* > newEliteNeighborhood;
        newEliteNeighborhood.push_back(_eliteBees[eliteHood]);
        ++bee;

        for (int eliteNeighbor = 0; eliteNeighbor < perEliteSite; eliteNeighbor++)
        {
            neighborBee = &_bees[bee];
            neighborBee->setRole(Bee::RECRUIT);
            newEliteNeighborhood.push_back(neighborBee);
            --bee;
        }
        _eliteNeighborhoods.push_back(newEliteNeighborhood);
        moveToSite(_eliteNeighborhoods.back());
    }

    for (int priorityHood = 0; priorityHood < prioritized; priorityHood++)
    {
        vector<Bee* > newPriorityNeighborhood;
        newPriorityNeighborhood.push_back(_priorityBees[priorityHood]);
        ++bee;

        for (int prioritizedNeighbor = 0; prioritizedNeighbor < perPrioritySite; prioritizedNeighbor++)
        {
            neighborBee = &_bees[bee];
            neighborBee->setRole(Bee::RECRUIT);
            newPriorityNeighborhood.push_back(neighborBee);
            --bee;
        }
        _priorityNeighborhoods.push_back(newPriorityNeighborhood);
        moveToSite(_priorityNeighborhoods.back());
    }

    emit quitRecruitmentThread();
    emit beesRecruited();
}

void WorkerBee::moveToSite(vector<Bee* > neighborhood)
{
    int fieldWidth = _fieldDims.width();
    int fieldHeight = _fieldDims.height();

    int deltaLambda = fieldWidth * _deltaLambda;
    deltaLambda /= 100;
    int deltaPhi = fieldHeight * _deltaPhi;
    deltaPhi /= 100;

    Bee* mainBee = neighborhood.front();

    int boundaryWest = mainBee->getPoint().x() - deltaLambda;

    if (boundaryWest < 0)
        boundaryWest = 0;

    int boundaryEast = mainBee->getPoint().x() + deltaLambda;

    if (boundaryEast >= fieldWidth)
        boundaryEast = fieldWidth - 1;

    int boundaryNorth = mainBee->getPoint().y() - deltaPhi;

    if (boundaryNorth < 0)
        boundaryNorth = 0;

    int boundarySouth = mainBee->getPoint().y() + deltaPhi;

    if (boundarySouth >= fieldHeight)
        boundarySouth = fieldHeight - 1;

    deltaLambda = boundaryEast - boundaryWest;
    deltaPhi = boundarySouth - boundaryNorth;

    // qDebug() << "evaluating neighborhood fitnesses";

    int hiveJ = _hive.getPoint().x();
    int hiveI = _hive.getPoint().y();

    double fieldPtQuality;
    double distance;
    double fitness;
    //int inc = 0;

    int newX;
    int newY;

    int seed = rand();

    for (vector<Bee* >::iterator i = neighborhood.begin() + 1; i != neighborhood.end(); ++i)
    {
        Bee* bee = (*i);

        newX = boundaryWest + (deltaLambda * r4_uniform_01(seed));

        newY = boundaryNorth + (deltaPhi * r4_uniform_01(seed));

        bee->setPoint(QPoint(newX, newY));

        distance = sqrt(pow(newX - hiveJ, 2) + pow(newY - hiveI, 2));
        fieldPtQuality = _field[newY][newX];

        fitness = fieldPtQuality / distance;
        //qDebug() << "bee " << inc << " fitness is " << fitness;
        //++inc;

        bee->setFitness(fitness);
    }
}

const vector<vector< Bee* > >* WorkerBee::getEliteNeighborhoods()
{
    return const_cast<const vector<vector< Bee* > >* >(&_eliteNeighborhoods);
}

const vector<vector< Bee* > >* WorkerBee::getPriorityNeighborhoods()
{
    return const_cast<const vector<vector< Bee* > >* >(&_priorityNeighborhoods);
}

void WorkerBee::setNeighborFitEvalMembs(QThread& thread)
{
    disconnectEverything(thread);
    connect(&thread, SIGNAL(started()), this, SLOT(evalNeighborFits()), Qt::UniqueConnection);
    connect(this, SIGNAL(quitNeighborFitEvalThread()), &thread, SLOT(quit()), Qt::UniqueConnection);
}

struct compareBees
{
    bool operator()(Bee* oneBee, Bee* anotherBee)
    {
        return (*oneBee).getFitness() < (*anotherBee).getFitness();
    }
};

void WorkerBee::evalNeighborFits()
{
    int inc = 0;
    int elite = 1;
    Bee* bee ;
    //for all the elite neighborhoods
    for (vector<vector<Bee* > >::iterator i = _eliteNeighborhoods.begin();
         i != _eliteNeighborhoods.end(); ++i)
    {
        inc = 0;

        sort((*i).begin(), (*i).end(), compareBees());
        reverse((*i).begin(), (*i).end());
        /*
        sort((*i).begin() + 1, (*i).end(), compareBees());
        reverse((*i).begin() + 1, (*i).end());
        */
        for (vector<Bee* >::iterator j = (*i).begin(); j != (*i).end(); ++j)
        {
            bee = (*j);
            //  qDebug() << "bee " << inc << " fitness is " << bee->getFitness()
            //           << " in elite neighborhood " << elite;

            if (j == (*i).begin() )
            {
                //    qDebug() << " and is the fittest neighbor";
                bee->setRole(Bee::PRIORITY);
            }
            else
            {
                //   qDebug() << " and is a neighbor";
                bee->setRole(Bee::RECRUIT);
            }
            ++inc;
        }

        ++elite;
    }

    int prior = 1;

    //for all the elite neighborhoods
    for (vector<vector<Bee* > >::iterator i = _priorityNeighborhoods.begin();
         i != _priorityNeighborhoods.end(); ++i)
    {
        inc = 0;

        sort((*i).begin(), (*i).end(), compareBees());
        reverse((*i).begin(), (*i).end());
        /*
        sort((*i).begin() + 1, (*i).end(), compareBees());
        reverse((*i).begin() + 1, (*i).end());
        */

        for (vector<Bee* >::iterator j = (*i).begin(); j != (*i).end(); ++j)
        {
            bee = (*j);
            // qDebug() << "bee " << inc << " fitness is " << bee->getFitness()
            //          << " in priority neighborhood " << prior;

            if (j == (*i).begin())
            {
                bee->setRole(Bee::PRIORITY);
                //    qDebug() << " and is the fittest neighbor";
            }
            else
            {
                //    qDebug() << " and is a neighbor";
                bee->setRole(Bee::RECRUIT);
            }

            ++inc;
        }
        ++prior;
    }
    emit quitNeighborFitEvalThread();
    emit neighborFitsEval();
}

void WorkerBee::newGenMembers(QThread& thread)
{
    disconnectEverything(thread);
    connect(&thread, SIGNAL(started()), this, SLOT(regenerate()), Qt::UniqueConnection);
    connect(this, SIGNAL(quitRegenThread()), &thread, SLOT(quit()), Qt::UniqueConnection);
}

void WorkerBee::regenerate()
{
    _bees.clear();

    time_t rawtime;
    time(&rawtime);

    // seed the rand num gen with the current time
    srand(rawtime);

    Bee bee;

    //for each elite neighborhood
    for (vector<vector<Bee* > >::iterator i = _eliteNeighborhoods.begin();
         i != _eliteNeighborhoods.end(); ++i)
    {
        for (vector<Bee* >::iterator j = (*i).begin(); j != (*i).begin() + 1; ++j)
        {
            bee = (*(*j));
            _bees.push_back(bee);
        }
    }

    //for all the elite neighborhoods
    for (vector<vector<Bee* > >::iterator i = _priorityNeighborhoods.begin();
         i != _priorityNeighborhoods.end(); ++i)
    {
        for (vector<Bee* >::iterator j = (*i).begin(); j != (*i).begin() + 1; ++j)
        {
            bee = (*(*j));
            _bees.push_back(bee);
        }
    }
    for (int i = _bees.size(); i < _population; i++)
    {
        Bee newBee(_fieldDims);
        _bees.push_back(newBee);
    }
    evaluateFitnesses();
    /*
    qDebug() << "evaluating new gen fitnesses";
    int beeJ;
    int beeI;

    int hiveJ = _hive.getPoint().x();
    int hiveI = _hive.getPoint().y();

    double fieldPtQuality;
    double distance;
    double fitness;
    int inc = 0;

    for (vector<Bee >::iterator i = _bees.begin(); i != _bees.end(); ++i)
    {
        beeJ = (*i).getPoint().x();
        beeI = (*i).getPoint().y();
        distance = sqrt(pow(beeJ - hiveJ, 2) + pow(beeI - hiveI, 2));
        fieldPtQuality = _field[beeI][beeJ];

        fitness = fieldPtQuality / distance;
        //qDebug() << "bee " << inc << " fitness is " << fitness;

        (*i).setFitness(fitness);
        ++inc;
    }
    sort(_bees.begin(), _bees.end());
    */

    emit quitRegenThread();
    emit regenerated();
}
