#!/usr/bin/env python3

"""kml_cleaner.py
Cleans up KML files generated from Garmin Pilot logs. Performs basic
transformations on camera tilt, roll, and heading.
"""

import argparse
import defusedxml.ElementTree as ET

if __name__ == "__main__":
    argument_parser = argparse.ArgumentParser()
    argument_parser.add_argument(
        "--roll_correction",
        type=float,
        help="degrees of roll correction (+ to roll right, - to roll left)",
        default=0
    )
    argument_parser.add_argument(
        "--tilt_correction",
        type=float,
        help="degrees of tilt correction (+ to tilt up, - to tilt down)",
        default=0
    )
    argument_parser.add_argument(
        "--heading_correction",
        type=float,
        help="degrees of heading correction (+ for west, - for east)",
        default=0
    )
    argument_parser.add_argument(
        "input_file",
        help="Input file to process"
    )
    argument_parser.add_argument(
        "output_file",
        help="Output file"
    )

    args = argument_parser.parse_args()

    kml = ET.parse(args.input_file)
    kml_root = kml.getroot()

    # Correct the roll
    for roll in kml_root.iter("{http://www.opengis.net/kml/2.2}roll"):
        # Garmin pilot flips the roll as required by KML, apparently
        roll.text = str(-(float(roll.text) + args.roll_correction))
    # Correct the tilt
    for tilt in kml_root.iter("{http://www.opengis.net/kml/2.2}tilt"):
        tilt.text = str(float(tilt.text) + args.tilt_correction)
    # Correct the heading
    for heading in kml_root.iter("{http://www.opengis.net/kml/2.2}heading"):
        heading.text = str(float(heading.text) + args.heading_correction)

    kml.write(args.output_file)