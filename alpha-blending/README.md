# Alpha blending
Task was to blend two bmp images with SSE instructions and without them. You can see all time profiling data in this folder. 

SSE only: 		3.89
Without SSE only:	26.70
Both, inline:		33.15
Both, not inline:	51.08 (3.92 for SSE, 47.16 for ordinary)