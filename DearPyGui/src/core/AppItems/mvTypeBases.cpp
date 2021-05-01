#include "mvTypeBases.h"
#include <utility>
#include "mvApp.h"
#include "mvLog.h"
#include "mvUtilities.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"

namespace Marvel {

	mvIntPtrBase::mvIntPtrBase(const std::string& name)
		: 
		mvAppItem(name)
	{
	}

	void mvIntPtrBase::setDataSource(const std::string& dataSource)
	{
		if (dataSource == m_source) return;
		m_source = dataSource;

		mvRef<mvAppItem> item = mvApp::GetApp()->getItemRegistry().getItem(dataSource);
		if (!item)
		{
			mvThrowPythonError(1000, "Source item not found.");
			return;
		}
		if (item->getValueType() != getValueType())
		{
			mvThrowPythonError(1000, "Values types do not match");
			return;
		}
		m_value = std::get<std::shared_ptr<int>>(item->getValue());
	}

	PyObject* mvIntPtrBase::getPyValue()
	{
		return ToPyInt(*m_value);
	}

	void mvIntPtrBase::setPyValue(PyObject* value)
	{
		*m_value = ToInt(value);
	}

	mvInt4PtrBase::mvInt4PtrBase(const std::string& name)
		: mvAppItem(name)
	{
	}

	PyObject* mvInt4PtrBase::getPyValue()
	{
		return ToPyIntList(m_value->data(), 4);
	}

	void mvInt4PtrBase::setPyValue(PyObject* value)
	{
		std::vector<int> temp = ToIntVect(value);
		while (temp.size() < 4)
			temp.push_back(0);
		std::array<int, 4> temp_array;
		for (int i = 0; i < temp_array.size(); i++)
			temp_array[i] = temp[i];
		if (m_value)
			*m_value = temp_array;
		else
			m_value = std::make_shared<std::array<int, 4>>(temp_array);
	}

	void mvInt4PtrBase::setDataSource(const std::string& dataSource)
	{
		if (dataSource == m_source) return;
		m_source = dataSource;

		mvRef<mvAppItem> item = mvApp::GetApp()->getItemRegistry().getItem(dataSource);
		if (!item)
		{
			mvThrowPythonError(1000, "Source item not found.");
			return;
		}
		if (item->getValueType() != getValueType())
		{
			mvThrowPythonError(1000, "Values types do not match");
			return;
		}
		m_value = std::get<std::shared_ptr<std::array<int, 4>>>(item->getValue());
	}

	mvFloatPtrBase::mvFloatPtrBase(const std::string& name)
		: mvAppItem(name)
	{
	}

	PyObject* mvFloatPtrBase::getPyValue()
	{
		return ToPyFloat(*m_value);
	}

	void mvFloatPtrBase::setPyValue(PyObject* value)
	{
		*m_value = ToFloat(value);
	}

	void mvFloatPtrBase::setDataSource(const std::string& dataSource)
	{
		if (dataSource == m_source) return;
		m_source = dataSource;

		mvRef<mvAppItem> item = mvApp::GetApp()->getItemRegistry().getItem(dataSource);
		if (!item)
		{
			mvThrowPythonError(1000, "Source item not found.");
			return;
		}
		if (item->getValueType() != getValueType())
		{
			mvThrowPythonError(1000, "Values types do not match");
			return;
		}
		m_value = std::get<std::shared_ptr<float>>(item->getValue());
	}

	mvDoublePtrBase::mvDoublePtrBase(const std::string& name)
		: mvAppItem(name)
	{
	}

	PyObject* mvDoublePtrBase::getPyValue()
	{
		return ToPyDouble(*m_value);
	}

	void mvDoublePtrBase::setPyValue(PyObject* value)
	{
		*m_value = ToDouble(value);
	}

	void mvDoublePtrBase::setDataSource(const std::string& dataSource)
	{
		if (dataSource == m_source) return;
		m_source = dataSource;

		mvRef<mvAppItem> item = mvApp::GetApp()->getItemRegistry().getItem(dataSource);
		if (!item)
		{
			mvThrowPythonError(1000, "Source item not found.");
			return;
		}
		if (item->getValueType() != getValueType())
		{
			mvThrowPythonError(1000, "Values types do not match");
			return;
		}
		m_value = std::get<std::shared_ptr<double>>(item->getValue());
	}

	mvFloat4PtrBase::mvFloat4PtrBase(const std::string& name)
		: mvAppItem(name)
	{
	}

	PyObject* mvFloat4PtrBase::getPyValue()
	{
		return ToPyFloatList(m_value->data(), 4);
	}

	void mvFloat4PtrBase::setPyValue(PyObject* value)
	{
		std::vector<float> temp = ToFloatVect(value);
		while (temp.size() < 4)
			temp.push_back(0.0f);
		std::array<float, 4> temp_array;
		for (int i = 0; i < temp_array.size(); i++)
			temp_array[i] = temp[i];
		if (m_value)
			*m_value = temp_array;
		else
			m_value = std::make_shared<std::array<float, 4>>(temp_array);
	}

	void mvFloat4PtrBase::setDataSource(const std::string& dataSource)
	{
		if (dataSource == m_source) return;
		m_source = dataSource;

		mvRef<mvAppItem> item = mvApp::GetApp()->getItemRegistry().getItem(dataSource);
		if (!item)
		{
			mvThrowPythonError(1000, "Source item not found.");
			return;
		}
		if (item->getValueType() != getValueType())
		{
			mvThrowPythonError(1000, "Values types do not match");
			return;
		}
		m_value = std::get<std::shared_ptr<std::array<float, 4>>>(item->getValue());
	}

	mvDouble4PtrBase::mvDouble4PtrBase(const std::string& name)
		: mvAppItem(name)
	{
	}

	PyObject* mvDouble4PtrBase::getPyValue()
	{
		return ToPyFloatList(m_value->data(), 4);
	}

	void mvDouble4PtrBase::setPyValue(PyObject* value)
	{
		std::vector<double> temp = ToDoubleVect(value);
		while (temp.size() < 4)
			temp.push_back(0.0);
		std::array<double, 4> temp_array;
		for (int i = 0; i < temp_array.size(); i++)
			temp_array[i] = temp[i];
		if (m_value)
			*m_value = temp_array;
		else
			m_value = std::make_shared<std::array<double, 4>>(temp_array);
	}

	void mvDouble4PtrBase::setDataSource(const std::string& dataSource)
	{
		if (dataSource == m_source) return;
		m_source = dataSource;

		mvRef<mvAppItem> item = mvApp::GetApp()->getItemRegistry().getItem(dataSource);
		if (!item)
		{
			mvThrowPythonError(1000, "Source item not found.");
			return;
		}
		if (item->getValueType() != getValueType())
		{
			mvThrowPythonError(1000, "Values types do not match");
			return;
		}
		m_value = std::get<std::shared_ptr<std::array<double, 4>>>(item->getValue());
	}

	mvColorPtrBase::mvColorPtrBase(const std::string& name)
		: mvAppItem(name)
	{
	}

	PyObject* mvColorPtrBase::getPyValue()
	{
		mvColor color = { 
			m_value->data()[0], 
			m_value->data()[1], 
			m_value->data()[2], 
			m_value->data()[3]
		};
		return ToPyColor(color);
	}

	void mvColorPtrBase::setPyValue(PyObject* value)
	{
		std::vector<float> temp = ToFloatVect(value);
		while (temp.size() < 4)
			temp.push_back(0.0f);
		std::array<float, 4> temp_array;
		for (int i = 0; i < temp_array.size(); i++)
			temp_array[i] = temp[i];
		if (m_value)
			*m_value = temp_array;
		else
			m_value = std::make_shared<std::array<float, 4>>(temp_array);
	}

	void mvColorPtrBase::setDataSource(const std::string& dataSource)
	{
		if (dataSource == m_source) return;
		m_source = dataSource;

		mvRef<mvAppItem> item = mvApp::GetApp()->getItemRegistry().getItem(dataSource);
		if (!item)
		{
			mvThrowPythonError(1000, "Source item not found.");
			return;
		}
		if (item->getValueType() != getValueType())
		{
			mvThrowPythonError(1000, "Values types do not match");
			return;
		}
		m_value = std::get<std::shared_ptr<std::array<float, 4>>>(item->getValue());
	}

	mvBoolPtrBase::mvBoolPtrBase(const std::string& name)
		: mvAppItem(name)
	{
	}

	PyObject* mvBoolPtrBase::getPyValue()
	{
		return ToPyBool(*m_value);
	}

	void mvBoolPtrBase::setPyValue(PyObject* value)
	{
		*m_value = ToBool(value);
	}

	void mvBoolPtrBase::setDataSource(const std::string& dataSource)
	{
		if (dataSource == m_source) return;
		m_source = dataSource;

		mvRef<mvAppItem> item = mvApp::GetApp()->getItemRegistry().getItem(dataSource);
		if (!item)
		{
			mvThrowPythonError(1000, "Source item not found.");
			return;
		}
		if (item->getValueType() != getValueType())
		{
			mvThrowPythonError(1000, "Values types do not match");
			return;
		}
		m_value = std::get<std::shared_ptr<bool>>(item->getValue());
	}

	mvStringPtrBase::mvStringPtrBase(const std::string& name)
		: mvAppItem(name)
	{
	}

	PyObject* mvStringPtrBase::getPyValue()
	{
		return ToPyString(*m_value);
	}

	void mvStringPtrBase::setPyValue(PyObject* value)
	{
		*m_value = ToString(value);
	}

	void mvStringPtrBase::setDataSource(const std::string& dataSource)
	{
		if (dataSource == m_source) return;
		m_source = dataSource;

		mvRef<mvAppItem> item = mvApp::GetApp()->getItemRegistry().getItem(dataSource);
		if (!item)
		{
			mvThrowPythonError(1000, "Source item not found.");
			return;
		}
		if (item->getValueType() != getValueType())
		{
			mvThrowPythonError(1000, "Values types do not match");
			return;
		}
		m_value = std::get<std::shared_ptr<std::string>>(item->getValue());
	}

	mvTimePtrBase::mvTimePtrBase(const std::string& name)
		: mvAppItem(name)
	{
	}

	PyObject* mvTimePtrBase::getPyValue()
	{
		return ToPyTime(*m_value);
	}

	void mvTimePtrBase::setPyValue(PyObject* value)
	{

		if (m_value)
			*m_value = ToTime(value);
		else
			m_value = {};

		ImPlot::GetGmtTime(*m_imvalue, m_value.get());
	}

	mvFloatVectPtrBase::mvFloatVectPtrBase(const std::string& name)
		: mvAppItem(name)
	{
	}

	PyObject* mvFloatVectPtrBase::getPyValue()
	{
		return ToPyList(*m_value);
	}

	void mvFloatVectPtrBase::setPyValue(PyObject* value)
	{
		*m_value = ToFloatVect(value);
	}

	void mvFloatVectPtrBase::setDataSource(const std::string& dataSource)
	{
		if (dataSource == m_source) return;
		m_source = dataSource;

		mvRef<mvAppItem> item = mvApp::GetApp()->getItemRegistry().getItem(dataSource);
		if (!item)
		{
			mvThrowPythonError(1000, "Source item not found.");
			return;
		}
		if (item->getValueType() != getValueType())
		{
			mvThrowPythonError(1000, "Values types do not match");
			return;
		}
		m_value = std::get<std::shared_ptr<std::vector<float>>>(item->getValue());
	}

	mvSeriesBase::mvSeriesBase(const std::string& name)
		: mvAppItem(name)
	{
	}

	PyObject* mvSeriesBase::getPyValue()
	{
		return ToPyList(*m_value);
	}

	void mvSeriesBase::setPyValue(PyObject* value)
	{
		*m_value = ToVectVectDouble(value);
		resetMaxMins();
		calculateMaxMins();
	}

	void mvSeriesBase::setDataSource(const std::string& dataSource)
	{
		if (dataSource == m_source) return;
		m_source = dataSource;

		mvRef<mvAppItem> item = mvApp::GetApp()->getItemRegistry().getItem(dataSource);
		if (!item)
		{
			mvThrowPythonError(1000, "Source item not found.");
			return;
		}
		if (item->getValueType() != getValueType())
		{
			mvThrowPythonError(1000, "Values types do not match");
			return;
		}
		m_value = std::get<std::shared_ptr<std::vector<std::vector<double>>>>(item->getValue());
		resetMaxMins();
		calculateMaxMins();
	}

	const std::pair<double, double>& mvSeriesBase::getMaxMin(int i) const
	{
		assert(i < m_maxMins.size());

		return m_maxMins[i];
	}

	void mvSeriesBase::calculateMaxMins()
	{

		static const std::vector<double>* xptr;

		for (auto& data : (*m_value.get()))
		{
			xptr = &data;
			if (xptr->empty())
			{
				m_maxMins.emplace_back(0.0, 0.0);
				continue;
			}
			double maxValue = (*xptr)[0];
			double minValue = (*xptr)[0];

			for (const auto& x : (*xptr))
			{
				if (x > maxValue) maxValue = x;
				if (x < minValue) minValue = x;
			}

			m_maxMins.emplace_back(maxValue, minValue);
		}
	}

	void mvSeriesBase::resetMaxMins()
	{
		m_maxMins.clear();
	}

	bool mvSeriesBase::isParentCompatible(mvAppItemType type)
	{
		if (type == mvAppItemType::mvPlot)
			return true;

		mvThrowPythonError(1000, "Item's parent must be plot.");
		MV_ITEM_REGISTRY_ERROR("Item's parent must be plot.");
		assert(false);
		return false;
	}

}