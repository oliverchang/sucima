function encodeSpinAngle(angle) {
  return ((parseInt(angle, 10) + 360) % 360) / 2;
}

function decodeSpinAngle(value) {
  let result = value * 2;
  if (result <= 180) {
    return result;
  }

  return result - 360;
}

class Ball {
  constructor(spin_angle, spin_strength, trajectory, position, speed) {
    this.spin_angle = spin_angle;
    this.spin_strength = spin_strength;
    this.trajectory = trajectory;
    this.position = position;
    this.speed = speed;
  }

  static decode(buffer) {
    let view = new DataView(buffer);
    return new Ball(
        decodeSpinAngle(view.getUint8(0)),
        view.getUint8(1),
        view.getUint8(2),
        view.getInt8(3),
        view.getUint8(4));
  }

  static get encodedSize() {
    return 5;
  }

  encode(result) {
    result[0] = encodeSpinAngle(this.spin_angle);
    result[1] = this.spin_strength;
    result[2] = this.trajectory;
    result[3] = this.position;
    result[4] = this.speed;

    return result.buffer;
  }
}

export default Ball;
