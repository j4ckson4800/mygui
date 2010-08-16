/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "precompiled.h"
#include "OptionPathControl.h"
/*#include "SkinManager.h"
#include "ActionManager.h"
#include <limits>*/

namespace tools
{

	OptionPathControl::OptionPathControl(MyGUI::Widget* _parent) :
		wraps::BaseLayout("OptionPathControl.layout", _parent)/*,
		mList(nullptr),
		mCreate(nullptr),
		mRename(nullptr),
		mDelete(nullptr),
		mTextFieldControl(nullptr)*/
	{
		/*assignWidget(mList, "List");
		assignWidget(mCreate, "Create");
		assignWidget(mRename, "Rename");
		assignWidget(mDelete, "Delete");

		mList->eventListChangePosition += MyGUI::newDelegate(this, &OptionPathControl::notifyChangePosition);
		mCreate->eventMouseButtonClick += MyGUI::newDelegate(this, &OptionPathControl::notifyCreate);
		mRename->eventMouseButtonClick += MyGUI::newDelegate(this, &OptionPathControl::notifyRename);
		mDelete->eventMouseButtonClick += MyGUI::newDelegate(this, &OptionPathControl::notifyDelete);

		SkinManager::getInstance().eventChangeList += MyGUI::newDelegate(this, &OptionPathControl::notifyChangeList);*/
	}

	OptionPathControl::~OptionPathControl()
	{
		/*SkinManager::getInstance().eventChangeList -= MyGUI::newDelegate(this, &OptionPathControl::notifyChangeList);

		hideTextField();

		mCreate->eventMouseButtonClick -= MyGUI::newDelegate(this, &OptionPathControl::notifyCreate);
		mRename->eventMouseButtonClick -= MyGUI::newDelegate(this, &OptionPathControl::notifyRename);
		mDelete->eventMouseButtonClick -= MyGUI::newDelegate(this, &OptionPathControl::notifyDelete);
		mList->eventListChangePosition -= MyGUI::newDelegate(this, &OptionPathControl::notifyChangePosition);*/
	}

	/*void OptionPathControl::notifyChangePosition(MyGUI::List* _sender, size_t _index)
	{
		SkinItem* item = nullptr;

		if (_index != MyGUI::ITEM_NONE)
			item = *mList->getItemDataAt<SkinItem*>(_index);

		SkinManager::getInstance().setItemSelected(item);
	}

	void OptionPathControl::notifyCreate(MyGUI::Widget* _sender)
	{
		//showTextField(nullptr);
		createItem(getNextFreeName());
	}

	void OptionPathControl::notifyRename(MyGUI::Widget* _sender)
	{
		SkinItem* item = SkinManager::getInstance().getItemSelected();
		if (item != nullptr)
			showTextField(item);
	}

	void OptionPathControl::notifyDelete(MyGUI::Widget* _sender)
	{
		SkinItem* item = SkinManager::getInstance().getItemSelected();
		if (item != nullptr)
		{
			MyGUI::Message* message = MyGUI::Message::createMessageBox(
				"Message",
				L"��������",
				L"�� �������?",
				MyGUI::MessageBoxStyle::IconQuest | MyGUI::MessageBoxStyle::Yes | MyGUI::MessageBoxStyle::No);
			message->eventMessageBoxResult += MyGUI::newDelegate(this, &OptionPathControl::notifyDeleteMessageBoxResult);
		}
	}

	void OptionPathControl::notifyDeleteMessageBoxResult(MyGUI::Message* _sender, MyGUI::MessageBoxStyle _style)
	{
		if (_style == MyGUI::MessageBoxStyle::Yes)
		{
			SkinItem* item = SkinManager::getInstance().getItemSelected();

			// �������� ��������� �� ���������
			SkinItem* prev = nullptr;
			SkinItem* next = nullptr;

			ItemHolder<SkinItem>::EnumeratorItem items = SkinManager::getInstance().getChildsEnumerator();
			while (items.next())
			{
				SkinItem* current = items.current();
				if (current == item)
				{
					if (items.next())
						next = items.current();
					break;
				}
				prev = current;
			}

			if (next != nullptr)
				SkinManager::getInstance().setItemSelected(next);
			else if (prev != nullptr)
				SkinManager::getInstance().setItemSelected(prev);

			SkinManager::getInstance().destroyChild(item);

			updateList();

			ActionManager::getInstance().setChanges(true);
		}
	}

	void OptionPathControl::updateList()
	{
		mList->setIndexSelected(MyGUI::ITEM_NONE);
		mList->removeAllItems();

		SkinItem* selectedItem = SkinManager::getInstance().getItemSelected();

		ItemHolder<SkinItem>::EnumeratorItem items = SkinManager::getInstance().getChildsEnumerator();
		while (items.next())
		{
			size_t index = mList->getItemCount();

			SkinItem* item = items.current();

			size_t count = getNameCount(item->getName());
			if (count == 1)
				mList->addItem(item->getName());
			else
				mList->addItem("#FF0000" + item->getName());

			mList->setItemDataAt(index, item);
			if (item == selectedItem)
				mList->setIndexSelected(index);
		}
	}

	void OptionPathControl::hideTextField()
	{
		if (mTextFieldControl != nullptr)
		{
			mTextFieldControl->hide();

			mTextFieldControl->eventResult = nullptr;

			delete mTextFieldControl;
			mTextFieldControl = nullptr;
		}
	}

	void OptionPathControl::showTextField(SkinItem* _item)
	{
		hideTextField();

		mTextFieldControl = new TextFieldControl();
		mTextFieldControl->setCaption(_item == nullptr ? "Create" : "Rename");
		mTextFieldControl->setTextField(_item == nullptr ? getNextFreeName() : _item->getName());
		mTextFieldControl->setUserData(_item);
		mTextFieldControl->show();

		mTextFieldControl->eventResult = MyGUI::newDelegate(this, &OptionPathControl::notifyTextFieldResult);
	}

	MyGUI::UString OptionPathControl::getNextFreeName()
	{
		MyGUI::UString pattern = "SkinName";

		for (size_t index=0; index<std::numeric_limits<size_t>::max(); index++)
		{
			MyGUI::UString name = MyGUI::utility::toString(pattern, index);
			if (!isNameExist(name))
				return name;
		}

		return "";
	}

	bool OptionPathControl::isNameExist(const MyGUI::UString& _value)
	{
		ItemHolder<SkinItem>::EnumeratorItem items = SkinManager::getInstance().getChildsEnumerator();
		while (items.next())
		{
			SkinItem* item = items.current();
			if (item->getName() == _value)
				return true;
		}
		return false;
	}

	size_t OptionPathControl::getNameCount(const MyGUI::UString& _value)
	{
		size_t result = 0;
		ItemHolder<SkinItem>::EnumeratorItem items = SkinManager::getInstance().getChildsEnumerator();
		while (items.next())
		{
			SkinItem* item = items.current();
			if (item->getName() == _value)
				++result;
		}
		return result;
	}

	void OptionPathControl::notifyTextFieldResult(bool _result)
	{
		if (_result)
		{
			SkinItem* item = *mTextFieldControl->getUserData<SkinItem*>();
			if (item != nullptr)
				renameItem(item, mTextFieldControl->getTextField());
			else
				createItem(mTextFieldControl->getTextField());
		}

		hideTextField();
	}

	void OptionPathControl::renameItem(SkinItem* _item, const MyGUI::UString& _value)
	{
		_item->setName(_value);
		updateList();

		ActionManager::getInstance().setChanges(true);
	}

	void OptionPathControl::createItem(const MyGUI::UString& _value)
	{
		SkinItem* item = SkinManager::getInstance().createChild();
		item->setName(_value);
		SkinManager::getInstance().setItemSelected(item);

		updateList();

		ActionManager::getInstance().setChanges(true);
	}

	void OptionPathControl::notifyChangeList()
	{
		updateList();
	}*/

} // namespace tools