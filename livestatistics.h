/**
 * Incremental Statistics computation
 *
 * - accumulates various statistics (mean, min, max, variance)
 * - allows combination of two sets for parallel or subset computation
 *
 * Requirements on T: 
 *  - zero assignment
 *  - compare < 
 *  - operators: + / * - 
 *
 * Using: Chan, Tony F.; Golub, Gene H.; LeVeque, Randall J. (1979), "Updating Formulae and a Pairwise Algorithm for Computing Sample Variances." (PDF), Technical Report STAN-CS-79-773, Department of Computer Science, Stanford University.

 *
 * Emanuele Ruffaldi @ Scuola Superiore Sant'Anna 2013-2015
 */
#pragma once

/// when T=int use B=float to have meaningful mean and variance
template <class T, class B = T>
struct LiveStatistics
{
    LiveStatistics() { reset(); }

    T vmin,vmax;
    T vsum;
    B vmean,vstd2;
    int vcount;

    /// returns true if 
    bool empty() const { return vcount == 0; }

    /// initial state
    void reset() { vcount = 0; vmin = vmax = vmean = vstd2 = vsum = vm2 = 0;  }

    /// adds value
    void add(T v);

    /// combines another 
    LiveStatistics & merge(const LiveStatistics<T,B> &other);

protected:
    B vm2;
};

template<class T, class B>
void LiveStatistics<T,B>::add(T v)
{
    if(vcount == 0)
    {
        vcount = 1;
        vmin = v;
        vmax = v;
        vsum = v;
        vmean = v;
        vstd2 = 0;
        vm2 = 0;
    }
    else
    {
        vcount++;
        if(v < vmin)
            vmin = v;
        if(!(v < vmax)) // assignment when equal, but we reduce requirement on <
            vmax = v;

        B dvold = v-vmean;
        vmean += dvold/vcount;
        vm2   += (v-vmean)*dvold;
        vsum  += v;

        vstd2 = vm2/(vcount-1);
    }
}

template<class T, class BB>
LiveStatistics<T,BB> & LiveStatistics<T,BB>::merge(const LiveStatistics<T,BB> &B) 
{
    if(B.empty())
        ;
    else if(empty())
        *this = B;
    else
    {
        // assert A.vcount >=1 and B.vcount >= 1 implies out.vcount >= 2
        if(B.vmin < vmin)
            vmin = B.vmin;
        if(!(B.vmax < vmax))
            vmax = B.vmax;

        int vcountold = vcount;
        BB delta = B.vmean-vmean;

        vcount += B.vcount;
        vsum += B.vsum;

        vmean += delta*B.vcount/vcount;
        vm2 += B.vm2 + delta*delta*(B.vcount*vcountold)/vcount;
        vstd2 = vm2/(vcount-1);
    }
    return *this;

}
