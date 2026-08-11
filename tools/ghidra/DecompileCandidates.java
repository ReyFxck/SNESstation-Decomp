// Decompile JAL targets which are not yet present in the progress manifest.
//
// Headless usage:
//   analyzeHeadless PROJECT_DIR PROJECT -process PROGRAM \
//     -scriptPath tools/ghidra -postScript DecompileCandidates.java \
//     analysis/jal_candidates.csv analysis/progress_targets.csv OUTPUT
//@category SNESstation

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.PrintWriter;
import java.util.HashSet;
import java.util.Set;

import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;

public class DecompileCandidates extends GhidraScript {
    private static final long CODE_START = 0x00100000L;
    private static final long CODE_END = 0x001b0800L;

    private static Set<Long> readFirstColumn(String path, boolean skipHeader) throws Exception {
        Set<Long> values = new HashSet<>();
        try (BufferedReader reader = new BufferedReader(new FileReader(path))) {
            String line;
            if (skipHeader) {
                reader.readLine();
            }
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

    private static Set<Long> readTrackedBeforeProgress16(String path) throws Exception {
        Set<Long> values = new HashSet<>();
        try (BufferedReader reader = new BufferedReader(new FileReader(path))) {
            reader.readLine();
            String line;
            while ((line = reader.readLine()) != null) {
                // Keep the script reproducible after the generated checkpoint has
                // been applied: its rows are outputs, not part of the P15 baseline.
                if (line.contains("Progress 16:")) {
                    continue;
                }
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

    private static long parseFirstCallSite(String row) {
        int lastComma = row.lastIndexOf(',');
        if (lastComma < 0) {
            return -1;
        }
        String field = row.substring(lastComma + 1).trim();
        if (!field.startsWith("0x")) {
            return -1;
        }
        return Long.parseUnsignedLong(field.substring(2), 16);
    }

    @Override
    public void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length != 3) {
            throw new IllegalArgumentException(
                "expected: JAL_CANDIDATES PROGRESS_TARGETS OUTPUT");
        }

        Set<Long> tracked = readTrackedBeforeProgress16(args[1]);
        DecompInterface decompiler = new DecompInterface();
        decompiler.toggleCCode(true);
        decompiler.toggleSyntaxTree(true);
        if (!decompiler.openProgram(currentProgram)) {
            throw new IllegalStateException("could not initialize the decompiler");
        }

        int attempted = 0;
        int completed = 0;
        try (BufferedReader reader = new BufferedReader(new FileReader(args[0]));
             PrintWriter output = new PrintWriter(args[2], "UTF-8")) {
            reader.readLine();
            String row;
            while ((row = reader.readLine()) != null && !monitor.isCancelled()) {
                int comma = row.indexOf(',');
                if (comma < 0) {
                    continue;
                }
                String addressField = row.substring(0, comma).trim();
                if (!addressField.startsWith("0x")) {
                    continue;
                }
                long value = Long.parseUnsignedLong(addressField.substring(2), 16);
                long caller = parseFirstCallSite(row);
                if (value < CODE_START || value >= CODE_END ||
                    caller < CODE_START || caller >= CODE_END || tracked.contains(value)) {
                    continue;
                }

                attempted++;
                Address address = currentProgram.getAddressFactory()
                    .getDefaultAddressSpace().getAddress(value);
                Function function = getFunctionAt(address);
                if (function == null) {
                    disassemble(address);
                    function = createFunction(address, "FUN_" + Long.toHexString(value));
                }

                output.printf("\n/* ===== 0x%08x ===== */%n", value);
                if (function == null) {
                    output.println("/* no function could be created */");
                    continue;
                }
                DecompileResults result = decompiler.decompileFunction(function, 45, monitor);
                if (result.decompileCompleted() && result.getDecompiledFunction() != null) {
                    output.println(result.getDecompiledFunction().getC());
                    completed++;
                } else {
                    output.println("/* decompiler failed: " + result.getErrorMessage() + " */");
                }
            }
            output.printf("%n/* attempted=%d completed=%d */%n", attempted, completed);
        } finally {
            decompiler.dispose();
        }
        println("Decompiled " + completed + " of " + attempted + " untracked code targets");
    }
}
