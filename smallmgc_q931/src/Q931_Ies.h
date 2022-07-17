/*
 * Q931_Ies.h
 *
 *  Created on: 10.03.2009
 *      Author: thomas
 */

#ifndef Q931_IES_H_
#define Q931_IES_H_
#include <vector>

class Q931Ies {
public:
	Q931Ies();
	Q931Ies(Q931Ies& copy);
	Q931Ies& operator=(const Q931Ies& copy);
	virtual ~Q931Ies();

	void flush();
	void destroy();
	void add(class Q931Ie *ie);
	void add_put(class Q931Ie *ie);
    void del(class Q931Ie *ie);
	void merge(Q931Ies *src_ies);
	void copy(Q931Ies *src_ies);
    Q931Ie* getIe(int count) { return ies[count]; };
	int get_count()
	{
		return count;
	}

	void sort();
	static int compare(const void *a, const void *b);
	static inline int q931_intcmp(int a, int b)
	{
	        if (a == b)
	                return 0;
	        else if (a > b)
	                return 1;
	        else
	                return -1;
	}

private:
	class std::vector<Q931Ie*>  ies;
	int count;
	int merged;
	int assigned;

};

#endif /* Q931_IES_H_ */
