
GPPPARAMS = -m32 -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore
ASPARAMS = --32
LDPARAMS = -m i386pe
OCPYPARAMS = -O elf32-i386

BUILD = build
SRC = src
INCLUDE = include

_OBJECTS = loader.o kernel.o gdt.o
OBJECTS = $(patsubst %, $(BUILD)/%, $(_OBJECTS))

$(BUILD)/%.o: $(SRC)/%.cpp
	g++ $(GPPPARAMS) -I $(INCLUDE) -o $@ -c $<

$(BUILD)/%.o: $(SRC)/%.asm
	as $(ASPARAMS) -o $@ $<

$(BUILD)/kernel.tmp: $(SRC)/linker.ld $(OBJECTS)
	ld $(LDPARAMS) -T $< -o $@ $(OBJECTS)

$(BUILD)/%.bin: $(BUILD)/%.tmp
	objcopy $(OCPYPARAMS) $< $@

$(BUILD)/%.iso: $(BUILD)/%.bin
	mkdir iso
	mkdir iso/boot
	mkdir iso/boot/grub
	cp $< iso/boot/
	echo set timeout=0 							>> iso/boot/grub/grub.cfg
	echo set default=0 							>> iso/boot/grub/grub.cfg
	echo 										>> iso/boot/grub/grub.cfg
	echo 'menuentry "SickOS" {' 				>> iso/boot/grub/grub.cfg
	echo "    multiboot /boot/$(notdir $<)" 	>> iso/boot/grub/grub.cfg
	echo "    boot" 							>> iso/boot/grub/grub.cfg
	echo } 										>> iso/boot/grub/grub.cfg
	bash -c "grub-mkrescue --output=$@ iso"
	rm -rf iso

run: $(BUILD)/kernel.iso
	-taskkill /F /IM "VirtualBox.exe"
	-taskkill /F /IM "VBoxManage.exe"
	-taskkill /F /IM "VirtualBoxVM.exe"
	VBoxManage startvm "SickOS Machine"
