#include "Long.h" 

bool Long::operator==(const Long & o) const
{
	if (size() == 1 && o.size() == 1 && a[0] == 0 && o[0] == 0)
		return true;
	if (sign != o.sign) return false;
	else if (size() != o.size()) return false;
	for (int i = 0; i < size(); ++i) {
		if (a[i] != o[i]) return false;
	}
	return true;
}
bool Long::operator!=(const Long & o) const
{
	return !operator==(o);
}
bool Long::operator<(const Long & o) const
{
	if (sign < o.sign) return true;
	else if (size() < o.size()) return true;
	else if (size() == o.size()) {
		for (int i = size() - 1; i >= 0; --i) {
			if (a[i] > o[i]) return false;
			else if (a[i] < o[i]) return true;
		}
		return false;
	}
	else return false;
}
bool Long::operator>(const Long & o) const
{
	if (sign > o.sign) return true;
	else if (size() > o.size())
		return true;
	else if (size() == o.size()) {
		for (int i = size() - 1; i >= 0; --i) {
			if (a[i] < o[i]) return false;
			else if (a[i] > o[i]) return true;
		}
		return false;
	}
	else return false;
}
bool Long::operator<=(const Long & o) const
{
	if (sign < o.sign) return true;
	else if (size() < o.size()) return true;
	else if (size() == o.size()) {
		for (int i = size() - 1; i >= 0; --i) {
			if (a[i] > o[i]) return false;
			else if (a[i] < o[i]) return true;
		}
		return true;
	}
	else return false;
}
bool Long::operator>=(const Long & o) const
{
	if (sign > o.sign) return true;
	else if (size() > o.size())
		return true;
	else if (size() == o.size()) {
		for (int i = size() - 1; i >= 0; --i) {
			if (a[i] < o[i]) return false;
			else if (a[i] > o[i]) return true;
		}
		return true;
	}
	else return false;
}
