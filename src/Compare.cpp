#include "Long.h" 

bool Long::operator==(const Long & other) const
{
	if (size() == 1 && other.size() == 1 && a[0] == 0 && other[0] == 0)
		return true;
	if (sign != other.sign) return false;
	else if (size() != other.size()) return false;
	for (int i = 0; i < static_cast<int>(size()); ++i) {
		if (a[i] != other[i]) return false;
	}
	return true;
}
bool Long::operator!=(const Long & other) const
{
	return !operator==(other);
}
bool Long::operator<(const Long & other) const
{
	if (sign < other.sign) return true;
	else if (size() < other.size()) return true;
	else if (size() == other.size()) {
		for (int i = size() - 1; i >= 0; --i) {
			if (a[i] > other[i]) return false;
			else if (a[i] < other[i]) return true;
		}
		return false;
	}
	else return false;
}
bool Long::operator>(const Long & other) const
{
	if (sign > other.sign) return true;
	else if (size() > other.size())
		return true;
	else if (size() == other.size()) {
		for (int i = size() - 1; i >= 0; --i) {
			if (a[i] < other[i]) return false;
			else if (a[i] > other[i]) return true;
		}
		return false;
	}
	else return false;
}
bool Long::operator<=(const Long & other) const
{
	if (sign < other.sign) return true;
	else if (size() < other.size()) return true;
	else if (size() == other.size()) {
		for (int i = size() - 1; i >= 0; --i) {
			if (a[i] > other[i]) return false;
			else if (a[i] < other[i]) return true;
		}
		return true;
	}
	else return false;
}
bool Long::operator>=(const Long & other) const
{
	if (sign > other.sign) return true;
	else if (size() > other.size())
		return true;
	else if (size() == other.size()) {
		for (int i = size() - 1; i >= 0; --i) {
			if (a[i] < other[i]) return false;
		else if (a[i] > other[i]) return true;
		}
		return true;
	}
	else return false;
}
