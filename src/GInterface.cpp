/*
 * Add those two methods CGInterface
 * CREDITS: Kyuubi
 */
 
int CGInterface::OnKeyDown(int keycode, int a3, int a4)
{
    switch (keycode) {
        case VK_TAB:
            m_IRM.GetResObj(5105,1)->ShowGWnd(true);
            break;
    }
    return reinterpret_cast<int (__thiscall *)(CGInterface *, int, int, int)>(0x00780610)(this, keycode, a3, a4);
}

void CGInterface::OnEscapePressed()
{
    if(m_IRM.GetResObj(5105,1) && m_IRM.GetResObj(5105,1)->IsVisible())
        m_IRM.GetResObj(5105,1)->ShowGWnd(false);

    reinterpret_cast<void (__thiscall *)(CGInterface *)>(0x0079d620)(this);
}