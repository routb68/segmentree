template <class T> class SegmentTree {
public:
	SegmentTree(vector<T> data, T value, T (*combine)(T obj1, T obj2));
	SegmentTree(T arr[], int n, T value, T (*combine)(T obj1, T obj2));
	T query(int l, int r);
	void update(int idx, T val);

private:
	T *tree;
	void constructTree(vector<T> data);
	int segTreeSize;
	T valueForExtraNodes;
	T (*combine)(T obj1, T obj2);
	int calculateSize(int n);
	T querycall(int l, int r, int st, int ed, int node);
};

template <class T>
SegmentTree<T>::SegmentTree(
	vector<T> data, T value, T (*combine)(T obj1, T obj2)) {
	this->combine = combine;
	valueForExtraNodes = value;
	segTreeSize = calculateSize(data.size());
	constructTree(data);
}

template <class T>
SegmentTree<T>::SegmentTree(
	T ar[], int n, T value, T (*combine)(T obj1, T obj2)) {
	this->combine = combine;
	valueForExtraNodes = value;
	segTreeSize = calculateSize(n);

	vector<T> data;
	for (int i = 0; i < n; i++)
		data.push_back(ar[i]);

	constructTree(data);
}

template <class T> int SegmentTree<T>::calculateSize(int n) {
	int pow2 = 1;
	while (pow2 < n) {
		pow2 = pow2 << 1;
	}
	return 2 * pow2 - 1;
}

template <class T> T SegmentTree<T>::query(int l, int r) {
	int st = 0, ed = segTreeSize / 2;
	return querycall(l, r, st, ed, 0);
}
template <class T>
T SegmentTree<T>::querycall(int l, int r, int st, int ed, int node) {
	if ((r < st) || (l > ed) || (l > r)) return valueForExtraNodes;
	if (st >= l && ed <= r) return tree[node];
	T leftVal = querycall(l, r, st, (st + ed) / 2, 2 * node + 1);
	T rightVal = querycall(l, r, (st + ed) / 2 + 1, ed, 2 * node + 2);
	return combine(leftVal, rightVal);
}

template <class T> void SegmentTree<T>::constructTree(vector<T> data) {
	int n = data.size();
	tree = new T[segTreeSize];
	int extraNodes = (segTreeSize / 2 + 1) - n;
	for (int i = segTreeSize - 1; i >= 0; i--) {
		if (extraNodes > 0) {
			tree[i] = valueForExtraNodes;
			extraNodes--;
		} else if (n > 0) {
			tree[i] = data[n - 1];
			n--;
		} else
			tree[i] = combine(tree[2 * i + 1], tree[2 * i + 2]);
	}
}

template <class T> void SegmentTree<T>::update(int idx, T val) {
	int segTreeIdx = (segTreeSize / 2) + idx;
	tree[segTreeIdx] = val;
	while ((segTreeIdx - 1) / 2 >= 0) {
		segTreeIdx = (segTreeIdx - 1) / 2;
		if (2 * segTreeIdx + 1 < segTreeSize)
			tree[segTreeIdx] =
				combine(tree[2 * segTreeIdx + 1], tree[2 * segTreeIdx + 2]);
		if (segTreeIdx == 0) break;
	}
}
