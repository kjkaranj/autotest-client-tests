import logging, time
from autotest_lib.client.common_lib import error
import kvm_subprocess, kvm_test_utils, kvm_utils


def run_boot(test, params, env):
    """
    KVM reboot test:
    1) Log into a guest
    2) Send a reboot command or a system_reset monitor command (optional)
    3) Wait until the guest is up again
    4) Log into the guest to verify it's up again

    @param test: kvm test object
    @param params: Dictionary with the test parameters
    @param env: Dictionary with test environment.
    """
    vm = env.get_vm(params["main_vm"])
    vm.verify_alive()
    timeout = float(params.get("login_timeout", 240))
    session = vm.wait_for_login(timeout=timeout)

    try:
        if not params.get("reboot_method"):
            return

        # Reboot the VM
        session = kvm_test_utils.reboot(vm, session,
                                    params.get("reboot_method"),
                                    float(params.get("sleep_before_reset", 10)),
                                    0, timeout)

    finally:
        session.close()
