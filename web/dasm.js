let reader = new FileReader();

function load_file() {
    let files = document.getElementById('file_input').files;
    let file = files[0];
    reader.addEventListener('loadend', print_file);
    reader.readAsArrayBuffer(file);
}

function print_file(e) {
    let result = reader.result;
    const uint8_view = new Uint8Array(result);

    FS.writeFile('riscvdasm.bin', uint8_view)
    Module.ccall('disassemble_file', 'number', ['string'], ['riscvdasm.bin'])
}

