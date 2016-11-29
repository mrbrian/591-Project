#ifndef _MYKDTREE_H_
#define _MYKDTREE_H_

class MyKDTree
{
public:
    MyKDTree(int k);
    void insert(photon* photon);
    void balance();
    void find_nearest(Vector3D pos);
    void find_nearest_n(Vector3D pos, int n);
}

#endif	/* _KDTREE_H_ */
