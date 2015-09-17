#ifndef LIVESTATISTICS_H
#define LIVESTATISTICS_H

template <class T>
struct LiveStatistics
{
    LiveStatistics() { reset(); }
    T vmin,vmax,vmean,vstd2;
    T vsum;
    int vcount;

    bool isEmpty() const { return vcount == 0; }

    void reset() { vcount = 0; vmin = vmax = vmean = vstd2 = vsum = vm2= 0;  }
    void update(T v);
    void merge(const LiveStatistics<T> &other);

protected:
    T vm2;
};

inline template<class T>
void LiveStatistics<T>::update(T v)
{
    if(vcount == 0)
    {
        vcount = 1;
        vmin = v;
        vmax = v;
        vstd2 = 0;
        vsum = v;
        vmean = v;
        vm2 = 0;
    }
    else
    {
        vcount++;
        if(v < vmin)
            vmin = v;
        if(v > vmax)
            vmax = v;

        float dvold = v-vmean;
        vmean += dvold/vcount;
        vm2 += (v-vmean)*dvold;
        vsum  += v;

        vstd2 = vm2/vcount;
    }
}

inline template<class T>
void LiveStatistics<T>::merge(const LiveStatistics<T> &other)
{
    if(other.isEmpty())
        return;
    else if(isEmpty())
        *this = other;
    else
    {
        if(other.vmin < vmin)
            vmin = other.vmin;
        if(other.vmax > vmax)
            vmax = other.vmax;

        int vcountold = vcount;
        T delta = vmean-other.vmean;

        vcount += other.vcount;
        vsum += other.vsum;
        vmean += delta*other.vcount/vcount;
        vm2 += other.vm2 + delta*delta*(other.vcount*vcountold)/vcount;
        vstd2 = vm2/vcount;
    }

}
