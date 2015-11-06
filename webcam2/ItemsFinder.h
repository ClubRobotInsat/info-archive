// ItemsFinder.h

#ifndef ITEMS_FINDER_H
#define ITEMS_FINDER_H

#include <list>

class ItemsFinder {
public:
	struct Item {
		int x, y, w, h;
		int centerX, centerY;
	};

	typedef std::list<Item> ItemsList;

	struct Config {
		int widthMin, heightMin;
		int widthMax, heightMax;
	};

private:
	ItemsList items;
	int width, height;

	Config config;

public:
	ItemsFinder(int width, int height);
	virtual ~ItemsFinder();

	void run(const char *dataGray);

	const ItemsList &getFoundItems() const;

	void setConfig(const Config &config);
	const Config &getConfig() const;
};

#endif // ITEMS_FINDER_H
