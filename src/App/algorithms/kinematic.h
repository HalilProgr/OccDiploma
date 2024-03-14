#ifndef KINEMATIC_H
#define KINEMATIC_H

#include <kdl/chain.hpp>
#include <kdl/jntarray.hpp>


class kinematic
{
public:
    kinematic();

    void init();

private:
    KDL::Chain chain;

    KDL::Frame cart,cartzero;
    KDL::JntArray jointInit;
    KDL::JntArray jointCur;
    KDL::JntArray jointMin;
    KDL::JntArray jointMax;
};

#endif // KINEMATIC_H
