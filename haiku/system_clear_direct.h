// Copyright Caz (2002) 
// Clear FrameBuffer
{
	static uint32 i = 0;
	static clipping_rect *clip;
	static int32 y = 0;//, x = 0;
	
	m_locker->Lock();
	if(m_connected) {
		clip = m_cliplist;
	for(i = 0; i < m_numcliprects; i++, clip++)
		for(y = 0; y < m_bounds.bottom - m_bounds.top + 1; y++)
		 if (clip->top - m_bounds.top <= y && clip->bottom - m_bounds.top >= y) {
		 	uint8 *dst = (uint8 *)(m_bits + ((y) * (m_rowbytes)) +((clip->left) * m_bitsperpixel));
			memset(dst, 0, (clip->right - clip->left + 1)* m_bitsperpixel);
		}
	}
	m_locker->Unlock();
}