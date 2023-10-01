#include "IFCircularMenu.h"
#include "Game.h"
#include "GInterface.h"
#include "GFXMainFrame/Controler.h"
#include "BSLib/multibyte.h"
#include <BSLib/Debug.h>

#define GDR_OVERLAY_TEXTURE 10
#define GDR_CIRCULAR_FRAME 11
#define GDR_SLOT_TITLE 12
#define GDR_SLOT_DESC 13

GFX_IMPLEMENT_DYNCREATE(CIFCircularMenu, CIFWnd)

GFX_BEGIN_MESSAGE_MAP(CIFCircularMenu, CIFWnd)
    ONG_VISIBLE_CHANGE()
    ONG_KEYUP()
GFX_END_MESSAGE_MAP()

CIFCircularMenu::CIFCircularMenu(void)
{

}

CIFCircularMenu::~CIFCircularMenu(void)
{

}

bool CIFCircularMenu::OnCreate(long ln)
{

    CIFWnd::OnCreate(ln);


    m_IRM.LoadFromFile("resinfo\\ifcircularmenu.txt");
    m_IRM.CreateInterfaceSection("Create", this);

    m_overlay =  m_IRM.GetResObj<CIFStatic>(GDR_OVERLAY_TEXTURE, 1);
    m_overlay->TB_Func_13("interface\\orbit\\orbit_screenfade.ddj", 0, 0);

    m_slotTitle = m_IRM.GetResObj<CIFStatic>(GDR_SLOT_TITLE, 1);
    m_slotTitle->SetGWndSize(500, 500);
    m_slotTitle->BringToFront();

    m_slotDescription = m_IRM.GetResObj<CIFStatic>(GDR_SLOT_DESC, 1);
    m_slotDescription->SetGWndSize(500, 500);
    m_slotDescription->BringToFront();

    RECT REC = { 0, 0, 0, 0 };

    m_circularFrame = (CIFCircularMenuFrame*)CGWnd::CreateInstance(this, GFX_RUNTIME_CLASS(CIFCircularMenuFrame), REC, GDR_CIRCULAR_FRAME, 0);

    m_circularArrow = (CIFStatic*)CGWnd::CreateInstance(this, GFX_RUNTIME_CLASS(CIFStatic), REC, GDR_OVERLAY_TEXTURE, 0);
    m_circularArrow->SetGWndSize(550, 550);
    m_circularArrow->TB_Func_13("interface\\orbit\\orbit_radialselector\\orbit_radialselector_0.ddj", 0, 0);

    m_selectedSlotId = 0;
    m_aSlotCount = 0;
    m_pSlots = new CIFCircularMenuSlot *[maxSlots];
    RegisterMenuSlot("interface\\orbit\\orbit_icons\\orbit_icon_default.ddj", L"Title for slotId 1", L"Description is here for slot 1");
    RegisterMenuSlot("interface\\orbit\\orbit_icons\\orbit_icon_1.ddj", L"Title for slotId 2", L"Description is here for slot 2");
    RegisterMenuSlot("interface\\orbit\\orbit_icons\\orbit_icon_2.ddj", L"Title for slotId 3", L"Description is here for slot 3");
    RegisterMenuSlot("interface\\orbit\\orbit_icons\\orbit_icon_3.ddj", L"Title for slotId 4", L"Description is here for slot 4");
    RegisterMenuSlot("interface\\orbit\\orbit_icons\\orbit_icon_4.ddj", L"Title for slotId 5", L"Description is here for slot 5");
    RegisterMenuSlot("interface\\orbit\\orbit_icons\\orbit_icon_5.ddj", L"Title for slotId 6", L"Description is here for slot 6");
    RegisterMenuSlot("interface\\orbit\\orbit_icons\\orbit_icon_6.ddj", L"Title for slotId 7", L"Description is here for slot 7");
    RegisterMenuSlot("interface\\orbit\\orbit_icons\\orbit_icon_7.ddj", L"Title for slotId 8", L"Description is here for slot 8");
    RegisterMenuSlot("interface\\orbit\\orbit_icons\\orbit_icon_8.ddj", L"Title for slotId 9", L"Description is here for slot 9");
    /*RegisterMenuSlot("interface\\orbit\\orbit_icons\\orbit_icon_9.ddj", L"Title for slotId 10", L"Description is here for slot 10");
    RegisterMenuSlot("interface\\orbit\\orbit_icons\\orbit_icon_11.ddj", L"Title for slotId 11", L"Description is here for slot 11");
    RegisterMenuSlot("interface\\orbit\\orbit_icons\\orbit_icon_12.ddj", L"Title for slotId 12", L"Description is here for slot 12");
    RegisterMenuSlot("interface\\orbit\\orbit_icons\\orbit_icon_13.ddj", L"Title for slotId 13", L"Description is here for slot 13");
    RegisterMenuSlot("interface\\orbit\\orbit_icons\\orbit_icon_14.ddj", L"Title for slotId 14", L"Description is here for slot 14");
    RegisterMenuSlot("interface\\orbit\\orbit_icons\\orbit_icon_15.ddj", L"Title for slotId 15", L"Description is here for slot 15");
    RegisterMenuSlot("interface\\orbit\\orbit_icons\\orbit_icon_16.ddj", L"Title for slotId 16", L"Description is here for slot 16");
    RegisterMenuSlot("interface\\orbit\\orbit_icons\\orbit_icon_17.ddj", L"Title for slotId 17", L"Description is here for slot 17");
    RegisterMenuSlot("interface\\orbit\\orbit_icons\\orbit_icon_18.ddj", L"Title for slotId 18", L"Description is here for slot 18");
    RegisterMenuSlot("interface\\orbit\\orbit_icons\\orbit_icon_19.ddj", L"Title for slotId 19", L"Description is here for slot 18");
    RegisterMenuSlot("interface\\orbit\\orbit_icons\\orbit_icon_20.ddj", L"Title for slotId 20", L"Description is here for slot 20");*/

    DrawCircularMenu();

    this->ShowGWnd(false);

    return true;
}

void CIFCircularMenu::DrawCircularMenu()
{
    screenCenterPoint.x = theApp.GetClientDimensionStuff().width * 0.5;
    screenCenterPoint.y = theApp.GetClientDimensionStuff().height * 0.5;
    m_overlay->SetGWndSize(theApp.GetClientDimensionStuff().width, theApp.GetClientDimensionStuff().height);
    m_overlay->MoveGWnd(0,0);
    m_circularFrame->MoveGWnd((screenCenterPoint.x - 550 * 0.5), (screenCenterPoint.y - 550 * 0.5));
    m_circularArrow->MoveGWnd((screenCenterPoint.x - 550 * 0.5), (screenCenterPoint.y - 550 * 0.5));
    m_slotTitle->MoveGWnd((screenCenterPoint.x  - 500 * 0.5), (screenCenterPoint.y - 500 * 0.5) - 120);
    m_slotDescription->MoveGWnd((screenCenterPoint.x  - 500 * 0.5), (screenCenterPoint.y - 500 * 0.5));
    WrapSlots();
}

void CIFCircularMenu::OnUpdate()
{
    POINT p;
    if (GetCursorPos(&p)) {
        ScreenToClient(theApp.GetHWnd(), &p);
        UpdateSlopeAngle(p.x ,p.y);
    }
}

int CIFCircularMenu::OnKeyUp(UINT nChar, UINT a2, UINT a3)
{
    if(nChar == VK_TAB) {
        SlotCallbackMethod(m_selectedSlotId);
        ShowGWnd(false);
    }
    return 0;
}

void CIFCircularMenu::RegisterMenuSlot(std::n_string icon, std::n_wstring title, std::n_wstring description)
{
    RECT REC = { 0, 0, 0, 0 };
    int slotId = m_aSlotCount;
    m_pSlots[slotId] = (CIFCircularMenuSlot*)CGWnd::CreateInstance(this, GFX_RUNTIME_CLASS(CIFCircularMenuSlot), REC, slotFirstId + slotId, 0);
    m_pSlots[slotId]->SetGWndSize(85, 85);
    m_pSlots[slotId]->SetSlotId(slotId);
    m_pSlots[slotId]->SetTitle(title);
    m_pSlots[slotId]->SetIcon(icon);
    m_pSlots[slotId]->SetDescription(description);
    m_aSlotCount++;
}

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
        case 2:
            g_pCGInterface->ToggleActionWnd();
            break;
        case 3:
            g_pCGInterface->TogglePartyWnd();
            break;
        case 4:
            g_pCGInterface->TogglePlayerInfoWnd();
            break;
        case 5:
            g_pCGInterface->ToggleApprenticeshipWnd();
            break;
        case 6:
            g_pCGInterface->ToggleEquipmentWnd();
            break;
        case 7:
            g_pCGInterface->ToggleInventoryWnd();
            break;
        case 8:
            g_pCGInterface->ToggleQuestNew();
            break;
        case 9:
            g_pCGInterface->ToggleSkillWnd();
            break;
        case 10:
            g_pCGInterface->ToggleActionWnd();
            break;
        case 11:
            g_pCGInterface->ToggleMainPopup();
            break;
        case 12:
            g_pCGInterface->ToggleActionWnd();
            break;
		// ... Up to 19
    }
}

void CIFCircularMenu::WrapSlots()
{
    int angle = 360 / m_aSlotCount, slotRadius = 220;
    int xCenter = screenCenterPoint.x - 85 * 0.5, yCenter =  screenCenterPoint.y - 85 * 0.5;
    for(int i = 0; i < m_aSlotCount ;i++)
        m_pSlots[i]->SetPosition(cos( (angle * i * PI ) / 180 ) * slotRadius + xCenter,
                                 sin((angle * i * PI ) / 180 ) * slotRadius + yCenter);
}

void CIFCircularMenu::UpdateSlopeAngle(int x, int y)
{
    int dX = (int)(screenCenterPoint.x - x);
    int dY = (int)(screenCenterPoint.y - y);
    int slopeTheta = atan2((float)dY , dX) * 180 / PI + 180;

    if(m_lastAngle != slopeTheta) {
        m_lastAngle = slopeTheta;

        std::n_wstring icon_path(L"");
        wchar_t buffer[256];

        swprintf_s(buffer, L"interface\\orbit\\orbit_radialselector\\orbit_radialselector_%d.ddj",
                   m_lastAngle == 360 ? 359 : m_lastAngle);
        icon_path = buffer;
        m_circularArrow->TB_Func_13(TO_NSTRING(icon_path), 0, 0);

        swprintf_s(buffer, L"interface\\orbit\\orbit_radialmenu\\orbit_radialmenu_%d.ddj",
                   m_lastAngle == 360 ? 359 : m_lastAngle);
        icon_path = buffer;
        m_circularFrame->SetIcon(TO_NSTRING(icon_path));

        // update selected slot
        int selectedSlot =  slopeTheta / (360 / m_aSlotCount);
        if(selectedSlot >= 0 && selectedSlot < m_aSlotCount )
            SetFocusedSlot(selectedSlot);
    }
}

void CIFCircularMenu::SetFocusedSlot(int slotId)
{
    m_selectedSlotId = slotId;

    for(int i = 0; i < m_aSlotCount; i++)
        m_pSlots[i]->SetFocus(false);

    m_pSlots[m_selectedSlotId]->SetFocus(true);
    m_slotTitle->SetText((m_pSlots[m_selectedSlotId]->GetTitle().c_str()));
    m_slotDescription->SetText(m_pSlots[m_selectedSlotId]->GetDescription().c_str());
}

int CIFCircularMenu::OnVisibleStateChange(int newstate, int a2)
{
    if(newstate == 1){
        DrawCircularMenu();
        m_overlay->sub_6526E0(0, 200, 0.4, 0.0, 1);
        for(int i = 0; i < m_aSlotCount; i++)
            m_pSlots[i]->SetFade(true);
    }
    return 0;
}

