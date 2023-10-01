/*
 * Register the two hoocks on the Setup() method
 * CREDITS: Kyuubi
 */
 
replaceAddr(0x00d7110a + 1, addr_from_this(&CGInterface::OnKeyDown));

replaceOffset(0x0078b6ce, addr_from_this(&CGInterface::OnEscapePressed));
