package xfyneui

import (
	"fyne.io/fyne/v2/data/binding"
)

// StatusController holds the 5 strings bound to the UI
type StatusController struct {
	Lines [5]binding.String
}

func (sd *StatusController) UpdateStatus(index int, val string) {
	if index >= 0 && index < 5 {
		sd.Lines[index].Set(val) // Use the field 'Lines' belonging to StatusController
	}
}
