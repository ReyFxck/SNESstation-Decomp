// Decompile the audited Progress 17 closure target set.
//
// Headless usage:
//   analyzeHeadless PROJECT_DIR PROJECT -process PROGRAM -noanalysis \
//     -scriptPath tools/ghidra -postScript DecompileProgress17.java \
//     analysis/progress17_targets.csv OUTPUT
//@category SNESstation

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.PrintWriter;
import java.util.Set;
import java.util.TreeSet;

import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;

public class DecompileProgress17 extends GhidraScript {
    private static final long CODE_START = 0x00100000L;
    private static final long CODE_END = 0x001b0800L;
    private static final int EXPECTED_TARGETS = 74;

    private static Set<Long> readTargets(String path) throws Exception {
        Set<Long> values = new TreeSet<>();
        try (BufferedReader reader = new BufferedReader(new FileReader(path))) {
            reader.readLine();
            String line;
            while ((line = reader.readLine()) != null) {
                int comma = line.indexOf(',');
                String field = comma >= 0 ? line.substring(0, comma) : line;
                field = field.trim();
                if (field.startsWith("0x")) {
                    values.add(Long.parseUnsignedLong(field.substring(2), 16));
                }
            }
        }
        return values;
    }

    @Override
    public void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length != 2) {
            throw new IllegalArgumentException("expected: TARGETS OUTPUT");
        }
        Set<Long> targets = readTargets(args[0]);
        if (targets.size() != EXPECTED_TARGETS) {
            throw new IllegalStateException(
                "expected " + EXPECTED_TARGETS + " targets, got " + targets.size());
        }

        DecompInterface decompiler = new DecompInterface();
        decompiler.toggleCCode(true);
        decompiler.toggleSyntaxTree(true);
        if (!decompiler.openProgram(currentProgram)) {
            throw new IllegalStateException("could not initialize the decompiler");
        }

        int completed = 0;
        try (PrintWriter output = new PrintWriter(args[1], "UTF-8")) {
            for (long value : targets) {
                if (monitor.isCancelled()) {
                    break;
                }
                if (value < CODE_START || value >= CODE_END) {
                    throw new IllegalStateException(
                        String.format("target outside code: 0x%08x", value));
                }
                Address address = currentProgram.getAddressFactory()
                    .getDefaultAddressSpace().getAddress(value);
                Function function = getFunctionAt(address);
                if (function == null) {
                    disassemble(address);
                    function = createFunction(address, "FUN_" + Long.toHexString(value));
                }

                output.printf("%n/* ===== 0x%08x ===== */%n", value);
                if (function == null) {
                    output.println("/* no function could be created */");
                    continue;
                }
                DecompileResults result = decompiler.decompileFunction(
                    function, 120, monitor);
                if (result.decompileCompleted() && result.getDecompiledFunction() != null) {
                    output.println(result.getDecompiledFunction().getC());
                    completed++;
                } else {
                    output.println(
                        "/* decompiler failed: " + result.getErrorMessage() + " */");
                }
            }
            output.printf(
                "%n/* attempted=%d completed=%d */%n", targets.size(), completed);
        } finally {
            decompiler.dispose();
        }
        println("Progress 17 decompiled " + completed + " of " + targets.size());
    }
}
