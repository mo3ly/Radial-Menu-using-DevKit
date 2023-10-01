# [RELEASE] Radial Menu using DevKit
![Image Description](https://i.epvpimg.com/YFgIcab.jpg)

## ❒ HI EVERYONE,

I saw a very cool feature on Electus Online called Orbit menu which has a range of commands around a circle. These commands make accessing game features easily. So, I managed to implement that feature using DevKit and Electus assets.

---

### Registering a new slot to the menu:
Call the \`RegisterMenuSlot\` method in the \`CIFCircularMenu::OnCreate()\` method.
* The max slot number you can have is 20.

**Code:**
```cpp
RegisterMenuSlot("icon_path", L"Title", L"Description");
```

---

### Callback method for every slot:
In the \`SlotCallbackMethod\` you can define a callback function for each slot in the menu.

**Code:**
```cpp
void CIFCircularMenu::SlotCallbackMethod(int slotId)
{
    // register a callback method for each slot here..
    switch (slotId) {
        case 0:
            g_pCGInterface->ShowMessage_Warning(L"SlotCallbackMethod is called 1");
            break;
        case 1:
            g_pCGInterface->ShowMessage_Notice(L"SlotCallbackMethod is called 2");
            break;
        // ... Up to 19
    }
}
```

---

### Menu usage:
- Hold the tab button while keeping the mouse arrow on a direction to any slot.
- When the tab button is pressed the selected slot will execute its command.

---

## Note:
This is not the best version of the feature and the code might have some problems, but at least it gives you an idea of how such a feature can be implemented.

---

## ❒ CREDITS

**Credits**
- (Florain0)
- (Kyuubi)
- (khaleed2010)

*The idea and the media assets belongs to ELECTUS ONLINE.*
